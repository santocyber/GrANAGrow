//#################################################################PING WDT
void setClock() {

  sntp_servermode_dhcp(1);    // (optional)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}


//##############################################################################



void startAPMode() {
    // Iniciar o modo AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP("GRANAConfig"); // Nome da rede AP
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Endereço IP do AP: ");
    Serial.println(myIP);

    // Inicializa o servidor web
    server.on("/", handleRoot);
    server.on("/scan", HTTP_GET, handleScan);
    server.on("/connect", handleConnect);
    server.begin();
    Serial.println("Servidor AP iniciado");
}

void startWebServer() {
    // Configura o certificado CA
    client.setCACert(caCert);
    setClock();

    // Inicializa o servidor web
    server.on("/", handleMain);
    server.on("/delete-wifi", HTTP_POST, deletewififile);

    server.begin();
    Serial.println("Servidor web iniciado");

    // Exibe informações sobre os servidores DNS
    IPAddress dns1 = WiFi.dnsIP(0); // Primeiro servidor DNS
    IPAddress dns2 = WiFi.dnsIP(1); // Segundo servidor DNS
    Serial.print("Servidor DNS primário: ");
    Serial.println(dns1);
    Serial.print("Servidor DNS secundário: ");
    Serial.println(dns2);
}








//##############################################################################

uint32_t notConnectedCounter = 0;
uint32_t ConnectedCounter = 0;



void checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Conexão Wi-Fi perdida. Tentando reconectar...");
        WiFi.reconnect();
        unsigned long startTime = millis();
        while (WiFi.status() != WL_CONNECTED) {
            // Obtenha as configurações DNS atualmente em uso
  IPAddress dns1 = WiFi.dnsIP(0); // Primeiro servidor DNS
  IPAddress dns2 = WiFi.dnsIP(1); // Segundo servidor DNS

  Serial.print("Servidor DNS primário: ");
  Serial.println(dns1);
  Serial.print("Servidor DNS secundário: ");
  Serial.println(dns2);
  setClock();
            // Aguarda a reconexão por até 10 segundos
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




//#########################SALVA INFO WIFI NO SPIFF

void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo, const String &usuario) {
  File file = SPIFFS.open("/wifi_credentials.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo para salvar as credenciais.");
    return;
  }

  file.println(ssid);
  file.println(password);
  file.println(nomedobot);
  file.println(geo);
  file.println(usuario);

  file.close();
}



void  loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo, String &usuario) {
  File file = SPIFFS.open("/wifi_credentials.txt", "r");
  if (!file) {
    Serial.println("Arquivo de credenciais n         o encontrado.");
    return;
  }

  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  nomedobot = file.readStringUntil('\n');
  geo = file.readStringUntil('\n');
  usuario = file.readStringUntil('\n');

  file.close();
}

  void deletewififile() {
String ssid0 ="";
String password0 ="";
String nomedobot0 ="";
String geo0 ="";
String usuario0 ="";
        
          saveWifiCredentials(ssid0,password0,nomedobot0,geo0,usuario0);

          server.send(200, "text/plain", "WiFi file deleted");

          ESP.restart();

}



void botaoreset(){

buttonState = digitalRead(buttonPin); // Lê o estado atual do botão

  // Se o estado do botão mudou
  if (buttonState != lastButtonState) {
    // Se o botão foi pressionado
    if (buttonState == LOW) {
      startTime = millis(); // Armazena o tempo atual
    }
    // Se o botão foi solto
    else {
      unsigned long pressDuration = millis() - startTime; // Calcula a duração do pressionamento
      // Se a duração do pressionamento for maior ou igual à duração do longo pressionamento
      if (pressDuration >= longPressDuration) {
        deletewififile(); // Chama a função deletewififile
      }
    }
  }

  lastButtonState = buttonState; // Atualiza o estado anterior do botão
  delay(10); // Pequeno atraso para estabilidade
    
    
    
    }
