//#################################################################PING WDT
void setClock() {
    sntp_servermode_dhcp(1); // (opcional)
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

    // Espera até que a hora da internet seja obtida com sucesso
    time_t timeout = time(nullptr) + 30; // Espere por 30 segundos para sincronizar
    while (time(nullptr) < timeout) {
        if (time(nullptr) > 0) {
            break; // Sai do loop se o tempo for sincronizado
        }
        delay(1000);
    }

  
}



//##############################################################################
//VOID CAM
#if (CAMERA == 1)

int minutos = timerfoto.toInt(); // Convertendo a string para inteiro
unsigned long intervalo_millis = minutos * 60 * 1000; // Convertendo minutos em milissegundos

void CAM(void*p){
//portVALID_STACK_MEM(pxStackBuffer); 
    
    configASSERT( ( uint32_t ) p == 1UL );
    
  // Block for 500ms.
const TickType_t xDelay = intervalo_millis / portTICK_PERIOD_MS;

  for( ;; )
  {
      // Simply toggle the LED every 500ms, blocking between each toggle.
         capturaimagem();

      
      
      vTaskDelay( xDelay );
  }

  }
#endif








//##############################################################################

uint32_t notConnectedCounter = 0;
uint32_t ConnectedCounter = 0;



void checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Conexao Wi-Fi perdida. Tentando reconectar...");
        WiFi.reconnect();
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED) {

  Serial.print("Servidor DNS1:");
  Serial.println(dns1);
  Serial.print("Servidor DNS2:");
  Serial.println(dns2);
  setClock();
            // Aguarda a reconexao por 10 segundos
            if (millis() - startTime > 10000) {
                Serial.println("Falha ao reconectar ao Wi-Fi.");
                break;
            }
            delay(500);
        }
    }
}

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


    if(Ping.ping("google.com", 3)) {
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

  Serial.print("Espaco total: ");
  Serial.print(totalBytes);
  Serial.println(" bytes");

  Serial.print("Espaco restante: ");
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





void botaoreset(){

buttonState = digitalRead(buttonPin); 

  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      startTime = millis(); // Armazena o tempo atual
      Serial.println("BOTAO RESET PRESSIONADO");

    }
    else {
      unsigned long pressDuration = millis() - startTime; 
      if (pressDuration >= longPressDuration) {
        Serial.println("WIFI APAGADO PELO BOTAO RESET");
        deletewififile(); 
      }
    }
  }

  lastButtonState = buttonState; 
  delay(10); // Pequeno atraso para estabilidade
    
    
    
    }



void funcaoestado() {
    //###############INICIA LOOP ESTADO
    Serial.println("INICIA FUNCAO ESTADO LOOP");
    loadfile(estado, telastatus, mensagemstatus, mensagem);
    estado.trim();
    timerautomatico.trim();

      // Verifica se o modo automático está ativado
  if (timerautomatico == "1") {
    Serial.println("Modo automático ativado NAO EXECUTA LOOP ESTADO");
       

        } else {

           if (estado == "liga") {
            //neopixelWrite(RGB_BUILTIN, 0, 255, 0);
            Serial.println("ligado pela função estado salvo na ROM");
            digitalWrite(RELAY1_PIN, HIGH);
        }

        if (estado == "desliga") {
           // neopixelWrite(RGB_BUILTIN, 255, 0, 0);
            Serial.println("desligado pela função estado salvo na ROM");
            digitalWrite(RELAY1_PIN, LOW);
        }
        
    Serial.println("EXECUTANDO LOOP ESTADO");
  }

  Serial.println("Saindo da função LOOP ESTADO");
  
}


void controlarRelayPeloTimer(String timerautomatico, String hrdesliga, String hrliga, int relayPin) {
  timerautomatico.trim();

  // Verifica se o modo automático está ativado
  if (timerautomatico == "1") {
    Serial.println("Modo automático ativado");


    // Obter a hora atual
    time_t now = time(nullptr);
    struct tm *timeinfo;
    timeinfo = localtime(&now);
    int currentHour = timeinfo->tm_hour;
    int currentMinute = timeinfo->tm_min;

    // Lógica para verificar se é hora de desligar e desligar
    int horaDesliga = hrdesliga.substring(0, 2).toInt();
    int minutoDesliga = hrdesliga.substring(3).toInt();

    // Lógica para verificar se é hora de ligar e ligar
    int horaLiga = hrliga.substring(0, 2).toInt();
    int minutoLiga = hrliga.substring(3).toInt();



    // Comparar hora e minuto com a hora atual
if ((currentHour > horaDesliga) || (currentHour == horaDesliga && currentMinute >= minutoDesliga)) {
  Serial.println("DESLIGANDO pelo TIMER");
  estado = "desliga";
  savefile(estado, telastatus, mensagemstatus, mensagem);
  digitalWrite(RELAY1_PIN, LOW);
  //digitalWrite(RGB_BUILTIN, LOW);
}
 else if ((currentHour > horaLiga) || (currentHour == horaLiga && currentMinute >= minutoLiga)) {
  Serial.println("LIGANDO pelo TIMER");
  estado = "liga";
  savefile(estado, telastatus, mensagemstatus, mensagem);
  digitalWrite(RELAY1_PIN, HIGH);
  //neopixelWrite(RGB_BUILTIN, 0, 0, 255);
}



  } else {
    Serial.println("Modo automático desativado");
  }

  Serial.println("Saindo da função controlarRelayPeloTimer");
}







void serial(){

  if (Serial.available() > 0) { // Verifica se há dados disponíveis no terminal serial
    char receivedChar = Serial.read(); // Lê o caractere recebido do terminal serial
    statetela += receivedChar; // Adiciona o caractere recebido à string statetela
          Serial.println("String recebida: " + statetela);
        statetela.trim();
    // Verifica se o caractere recebido é uma quebra de linha (indicando o final da entrada)
    if (receivedChar == '\n') {
      // Se sim, imprime a string statetela no terminal serial
      Serial.println("String recebida: " + statetela);
      // Limpa a string statetela para coletar uma nova entrada
    }
     if (receivedChar == '0') {
      // Se sim, imprime a string statetela no terminal serial
      Serial.println("LIMPANDO String recebida: " + statetela);
      statetela = "";
      functionExecuted = false;

      // Limpa a string statetela para coletar uma nova entrada
    }
  }



}
