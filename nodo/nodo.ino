// —-----------------------------------------------------//
// Archivo nodo.ino
// version 1.11
// RadioHead Library - RHMesh Class
// Funciona para Heltec LoRa v2 y Arduino
// UADE PFI 
// Integrantes: Damian del Campo y Jonatan Salini
// —-----------------------------------------------------//

#define NODO_ID 2 // Node ID
#define RH_MESH_MAX_MESSAGE_LEN 50
#include <RHMesh.h>
#include <RH_RF95.h>

#define ESTACION_BASE 1
#define NODO_2 2
#define NODO_3 3
#define NODO_4 4

#ifdef ESP32
  #define RXTIMEOUT 150000  // It is roughly the delay between successive transmissions
  #define MAXCOUNTER 1000  // Max value for the Hello Message ID
  
  //Wifi config
  const char* wifi_ssid = "Speedy-Fibra-Home";
  const char* wifi_password =  "Paulo2019";
  //const char* wifi_ssid = "Chewy-Fi 2.4GHz";
  //const char* wifi_password =  "d3lc4mp0";
  
  //MQTT config
  const char* mqtt_server = "broker.hivemq.com";
  const int mqtt_port = 1883;
  const char* dataTopic = "mesh_gateway/data";
  
  // Heltec PINs
  #define LLG_CS  18
  #define LLG_INT 26
  RH_RF95 rf95(LLG_CS, LLG_INT); // Singleton instance of the radio driver
#else
  #define RXTIMEOUT 15000  // It is roughly the delay between successive transmissions
  #define MAXCOUNTER 1000  // Max value for the Hello Message ID
  // Arduino PINs
  #define RFM95_CS 10
  #define RFM95_RST 9
  #define RFM95_INT 2
  RH_RF95 rf95(RFM95_CS, RFM95_INT); // Singleton instance of the radio driver
#endif

RHMesh manager(rf95, NODO_ID); // Class to manage message delivery and receipt, using the driver declared above
#ifdef ESP32
  #include "WiFi.h"
  #include <PubSubClient.h>
  #include "heltec.h"
  #include "UADE.h"
  
  WiFiClient espClient;
  PubSubClient mqtt_client(espClient);
  
  //UADE Logo upload to Heltec Display
  void logo(){
    Heltec.display->clear();
    Heltec.display->display();
    Heltec.display -> drawXbm(0,0,logo_width,logo_height,(const unsigned char *)logo_bits);
    Heltec.display->display();
  }
  
  // Connect to WiFi
  void WIFISetUp(void)
  {
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid,wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println(F("Connecting wifi..."));
      }
      Serial.println(F("Connecting wifi...OK!"));
  }
  
  // Connect to MQTT broker
  void MQTTSetUp() {  
      mqtt_client.setServer(mqtt_server, mqtt_port);
      Serial.println(F("Connecting to MQTT..."));
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      if (mqtt_client.connect(clientId.c_str())) {
        Serial.println(F("Connecting to MQTT...OK!"));
        mqtt_client.subscribe(dataTopic);
      } else {
        Serial.print(F("failed with state "));
        Serial.print(mqtt_client.state());    
      }
  }
  void MQTTreconnect() {
    while (!mqtt_client.connected()) {
        Serial.println(F("Reconnecting to MQTT Broker..."));
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);   
        if (mqtt_client.connect(clientId.c_str())) {
          Serial.println(F("Reconnecting to MQTT Broker...OK!"));
          // subscribe to topic
          mqtt_client.subscribe(dataTopic);
        } 
    }
  }
#endif

uint8_t buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t data_rx[] = "And hello back to you from Nodo ";
unsigned int counter = 0; 
unsigned int rxCounter = 0; 

void sendMessageCustom(uint8_t destinationID,uint8_t* dataToSend, uint8_t lenDataToSend){
  Serial.print(F("Sending message to: NODO_"));
  Serial.println(destinationID);

  #ifdef ESP32
    Heltec.display->clear();
    Heltec.display->display();
    Heltec.display->drawString(0,0, "Sending message to: ");
    Heltec.display->drawString(0,10, "NODO_");
    Heltec.display->drawString(35,10, (String)destinationID);
    Heltec.display->display();
  #endif
  
  manager.printRoutingTable();  
  Serial.println(F("route printed"));
  if (manager.sendtoWait(dataToSend, lenDataToSend, destinationID) == RH_ROUTER_ERROR_NONE)
  {
    uint8_t len = sizeof(buf);
    uint8_t from;    
    if (manager.recvfromAckTimeout(buf, &len, 3000, &from))
    {
      Serial.print(F("got reply from : 0x"));
      Serial.print(from, HEX);
      Serial.print(F(": "));
      Serial.print((char*)buf);
      Serial.print(F(" rssi: "));
      Serial.println(rf95.lastRssi()); 
      #ifdef ESP32
        Heltec.display->drawString(0,20, "Send OK. Got reply");
        Heltec.display->display();
      #endif
    }
    else{
      #ifdef ESP32
        Heltec.display->drawString(0,20, "No reply");
        Heltec.display->display();
      #endif    
      Serial.println(F("No reply, is NODO_1, NODO_2 or NODO_3 running?"));
    }
  }
  else{
    #ifdef ESP32
      Heltec.display->drawString(0,20, "Send failed");
      Heltec.display->display();
    #endif
    Serial.println(F("sendtoWait failed. Are the intermediate mesh servers running?"));  
  }
}

void waitForMessage(){
    uint8_t len = sizeof(buf);
    uint8_t from;
    memset(buf,NULL,len);
    if (manager.recvfromAck(buf, &len, &from))
    {
      Serial.print(F("got request from : 0x"));
      Serial.print(from, HEX);
      Serial.print(F(": "));
      Serial.print((char*)buf);
      Serial.print(F(" rssi: "));
      Serial.println(rf95.lastRssi()); 
      
      manager.printRoutingTable();  
      Serial.println(F("route printed"));

      #ifdef ESP32
      if(NODO_ID == 1){ //Check if NODO_ID = Estacion Base, then prepare and send message to mqtt broker
        String mensaje = (char*)buf;
        mensaje=(String)from+(String)ESTACION_BASE+mensaje;
        if (!mqtt_client.connected()) MQTTreconnect();
        mqtt_client.loop();
        mqtt_client.publish(dataTopic,mensaje.c_str());
      }
      #endif
      
      // Send a reply back to the originator client
      if (manager.sendtoWait(data_rx, sizeof(data_rx), from) != RH_ROUTER_ERROR_NONE)
        Serial.println(F("sendtoWait failed"));
    }
}
