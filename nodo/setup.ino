// —-----------------------------------------------------//
// Archivo setup.ino
// version 1.11
// RadioHead Library - RHMesh Class
// Funciona para Heltec LoRa v2 y Arduino
// UADE PFI 
// Integrantes: Damian del Campo y Jonatan Salini
// —-----------------------------------------------------//
void setup() 
{
  Serial.begin(115200);
  #ifdef __AVR__
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
  #endif
  #ifdef ESP32
    if(NODO_ID == 1){
      Heltec.display->init();
      Heltec.display->setFont(ArialMT_Plain_10);
      logo();
      WIFISetUp();
      MQTTSetUp();
    }
    else{
      Heltec.display->init();
      Heltec.display->setFont(ArialMT_Plain_10);
      logo();
      delay(1000);
      Heltec.display->clear();
      Heltec.display->display();    }
  #endif
  while (!rf95.init()) {
    Serial.println(F("LoRa Radio Driver init failed"));
  }
  Serial.println(F("LoRa Radio Driver init OK!"));
  Serial.print(F("initializing node "));
  Serial.print(NODO_ID);
  if (!manager.init()) {
   Serial.println(F(" init failed"));
  } else {
   Serial.println(F(" done"));
  }
  //Double check node ID
  Serial.print(F("Check Nodo ID: "));
  Serial.println(manager.thisAddress());
  rf95.setTxPower(10, false); 
  rf95.setFrequency(915.0);
  rf95.setCADTimeout(500);
  if (!rf95.setModemConfig(RH_RF95::Bw125Cr45Sf128))Serial.println(F("set config failed"));
  Serial.println(F("RF95 driver ready"));

  #ifdef ESP32
    if(NODO_ID == 1){
      delay(100);
      Heltec.display->clear();
      Heltec.display->display();
    }
  #endif

}
