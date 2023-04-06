// —-----------------------------------------------------//
// Archivo loop.ino
// version 1.11
// RadioHead Library - RHMesh Class
// Funciona para Heltec LoRa v2 y Arduino
// UADE PFI 
// Integrantes: Damian del Campo y Jonatan Salini
// —-----------------------------------------------------//
void loop()
{
    if((NODO_ID != 1)){
      if(rxCounter++ >= RXTIMEOUT){
      // Variables to send over the mesh network
      String str1 = "Hello #"; 
      String str2 = String(counter); 
      String strConcat = str1 + str2; // Concatenate the two strings 
      int strLength = strConcat.length(); // Get the length of the concatenated string
      uint8_t result[strLength]; // Create an array of uint8_t the size of the concatenated string
      for (int i = 0; i < strLength; i++) { 
        result[i] = (uint8_t) strConcat.charAt(i); // Convert each character of the concatenated string to a uint8_t and add it to the array
      }
      rxCounter=0;
      sendMessageCustom(ESTACION_BASE,result,sizeof(result));
      //sendMessageCustom(NODO_2,result,sizeof(result));
      //sendMessageCustom(NODO_3,result,sizeof(result));
      if(counter++ >= MAXCOUNTER){
      counter=0;
      }
    }  
  }
  waitForMessage(); 
}
