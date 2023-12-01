//#################################################################PING WDT


uint32_t notConnectedCounter = 0;
uint32_t ConnectedCounter = 0;





void ensureWiFiConnection() {
    HTTPClient http;
    http.end();
   delay(1000);
   WiFi.disconnect();
   delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconectando ao Wi-Fi...");
    WiFi.disconnect();
    delay(1000);
    WiFi.begin(ssid, password); 
    delay(1000); // Aguarde 5 segundos para se reconectar
  }
}


void pingando(){

   Serial.print("Task PING running on core ");
   Serial.println(xPortGetCoreID());

    if(Ping.ping("google.com", 1)) {
      ConnectedCounter++;
 Serial.println("Ping pong OK contando: ");
 Serial.println(ConnectedCounter);
 Serial.println("Falhas do Ping: ");
 Serial.println(notConnectedCounter);

    Serial.print(Ping.averageTime());
    Serial.println(" ms");

    Serial.printf("\n%lu: Remaining free mem: %u\n", millis(), ESP.getFreeHeap());
    Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
   
  Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());
  
  size_t totalBytes = SPIFFS.totalBytes();
  size_t usedBytes = SPIFFS.usedBytes();

  Serial.print("Espaço total: ");
  Serial.print(totalBytes);
  Serial.println(" bytes");

  Serial.print("Espaço restante: ");
  Serial.print(totalBytes - usedBytes);
  Serial.println(" bytes");
      }

else{ 
      notConnectedCounter++;
          Serial.println("Falha do ping da internet, contando...");
          Serial.println(notConnectedCounter);

    if(notConnectedCounter > 5000) { // Reset a placa apos 5000 erros    
    Serial.println("Reiniciando esp por falha na internet");
    delay(4000);
          ESP.restart();
     
   }
   }

}

//##############VOID TELEGRAM

void TELE(void*p){
//portVALID_STACK_MEM(pxStackBuffer); 
    
    configASSERT( ( uint32_t ) p == 1UL );
    
  // Block for 500ms.
const TickType_t xDelay = 60000 / portTICK_PERIOD_MS;

  for( ;; )
  {
      // Simply toggle the LED every 500ms, blocking between each toggle.
  readTel();
      vTaskDelay( xDelay );
  }

  }


void readTel(){



   Serial.print("Task TELE running on core ");
   Serial.println(xPortGetCoreID());

    if (StateUpdate == "ativo") {

    telemetria();
  //  verifyActionAndExecute();
    pingando();

  }

esp_get_free_heap_size();
 
//  vTaskDelete(NULL);
       //   vTaskDelete(teletask);
   // vTaskSuspend(NULL);

}
