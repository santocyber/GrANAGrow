

//########################################TELEMETRIA
     void telemetria(){
      HTTPClient https;
      obterHoraEDataAtual();
      delay(500);
          
 String CHAVE2PHP = url;

  Serial.print("INICIA GRAVACAO TELEMETRIA...\n");

  if (https.begin(CHAVE2PHP)) {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
  // Construa o corpo da solicitação POST
  String postData = "mac_value=" + WiFi.macAddress();
  postData += "&iplocal=" + WiFi.localIP().toString();
  postData += "&granaentrada=" + String(contagrana);
  postData += "&objsaida=" + String(contaobj);
  postData += "&wifilocal=" + WiFi.SSID();
  postData += "&horalocal=" + horaAtual;
  postData += "&datalocal=" + dataAtual;
  postData += "&nomedobot=" + nomedobot;
  postData += "&geo=" + geo;

    int httpCode = https.POST(postData);
      Serial.println(postData);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been sent and Server response header has been handled
      Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
      String payload = https.getString();
      Serial.println(payload);
    }

Serial.println("Hora atual: " + horaAtual);
Serial.println("Data atual: " + dataAtual);

    Serial.println("");
    Serial.println(F("Desconectando."));
    https.end();

    digitalWrite(RGB_BUILTIN, LOW);
  } else {
    Serial.println(F("Falha na conexão"));
    Serial.println();
    neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Red
  }
}



//#######################################################

void funcaocreditoleapaga()  {
  HTTPClient http;
   while (true) {
  // Inicie a conexão HTTP uma vez, em vez de duas vezes
  http.begin(urlcredito + "?mac=" + WiFi.macAddress());

  // Faça a solicitação GET para a API PHP
  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    payload.trim();
    Serial.println("Resposta da API: " + payload);

    // Parse o JSON payload para obter as informações de crédito
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.println("Erro ao analisar JSON: " + String(error.c_str()));
    } else {
      int credit = doc["credito"];
            
      Serial.println("Valor do campo 'credito' no JSON:");
      Serial.println(credit);

      // Controlar o LED para piscar 'credito' vezes
      for (int i = 0; i < credit; i++) {
        neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // Verde
        delay(300); // Aguardar 300 ms (0,3 segundo)
        neopixelWrite(RGB_BUILTIN, 0, 0, 0);
        delay(300); // Aguardar 300 ms (0,3 segundo)
      }

      // Atualizar a variável 'contagrana' e chamar a função 'contagrana_data'
      contagrana += credit;
      contagrana_data(contagrana);
      Serial.println("Contagrana: ");
      Serial.println(contagrana);

      if (credit >= 1) {
        // Se o crédito for maior ou  igual a 1, saia do loop
       http.end();
       http.begin(url6 + "?mac=" + WiFi.macAddress());
       int httpCode = http.GET();

       // Check the HTTP status code
       if (httpCode == 200) {
       // The action was found
       String payload = http.getString();
       Serial.println(payload);
       http.end();
       delay(2000); // Aguardar 2000 ms (2,0 segundo)

       http.begin(urlcredito + "?mac=" + WiFi.macAddress());
       http.end();
       delay(2000); // Aguardar 2000 ms (2,0 segundo)


  }
        break;
      
      
      }}}}
      

  // Sempre chame http.end() para liberar recursos
  http.end();
}


    




//##########################################VERIFICA FUNCOES PENDENTES NO SQL

void verifyActionAndExecute() {
  HTTPClient httpClient;
  httpClient.begin(url6 + "?mac=" + WiFi.macAddress());
  int httpCode = httpClient.GET();

  // Check the HTTP status code
  if (httpCode == 200) {
    // The action was found
    String payload = httpClient.getString();
    Serial.println(payload);

    // Parse the JSON payload to get the action information
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    String action = doc["acoes"].as<String>();
    action.trim();
    
     if (estado != action) {
      Serial.println("SALVANDO NA ROM");
      savefile(action, mensagem);
      delay(200);
      Serial.println("LENDO ROM");
      loadfile(estado, mensagem);
      estado.trim();
     }

    // Execute the 
    if (estado == "ligado") {
      Serial.println("LIGADO PELO SQL");
      digitalWrite(relayPin3, HIGH);
      neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0); // Green
    } else if (estado == "desligado") {
      Serial.println("DESLIGADO PELO SQL");
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);
      digitalWrite(relayPin3, LOW);
    } else if (estado == "credito") {
      Serial.println("CREDITO PELO SQL");
      funcaocreditoleapaga();
      action = "ligado";
      estado = "ligado";
      savefile(action, mensagem);
      neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0); // Red
      delay(400);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);
       
    } else if (estado == "reinicia") {
      Serial.println("REINICIA PELO SQL");
      digitalWrite(relayPin3, LOW);
      neopixelWrite(RGB_BUILTIN, 255, 255, 127); // Yellow    
      delay(2000);
      neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Pink
      digitalWrite(relayPin3, HIGH);
      action = "ligado";
      savefile(action, mensagem);

}


 else {
      Serial.println("Unknown action: " + estado);
      Serial.println(WiFi.macAddress());

    }
  } else {
    // The action was not found
    Serial.println("Action not found: " + WiFi.macAddress());
  }

  httpClient.end();
}


//################################ Função para salvar o SSID e a senha no SPIFFS
void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo) {
  File file = SPIFFS.open("/wifi_credentials.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo para salvar as credenciais.");
    return;
  }

  file.println(ssid);
  file.println(password);
  file.println(nomedobot);
  file.println(geo);

  file.close();
}

// Função para carregar o SSID e a senha do SPIFFS
void  loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo) {
  File file = SPIFFS.open("/wifi_credentials.txt", "r");
  if (!file) {
    Serial.println("Arquivo de credenciais não encontrado.");
    return;
  }

  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  nomedobot = file.readStringUntil('\n');
  geo = file.readStringUntil('\n');

  file.close();
}

  void deletewififile() {
String ssid0 ="";
String password0 ="";
String nomedobot0 ="";
String geo0 ="";
        
          saveWifiCredentials(ssid0,password0,nomedobot0,geo0);
          ESP.restart();

}
//#########################SALVANDO e apagando CONTADORES EM ARQUIVOS

void delete_contagrana_data_file() {

          contagrana = 0;
          contagrana_data(contagrana);
}
void delete_contaobj_data_file() {
          contaobj = 0;
          contaobj_data(contaobj);
}
void deletenvs() {
          last_id = 0;
          save_nvs_data(last_id);
}

uint32_t read_contagrana(){
uint32_t contagrana = 0;
  if (SPIFFS.begin(true))
  {
    File contagranaFile = SPIFFS.open("/contagrana.txt", "r");
    if (contagranaFile)
    {
      contagrana = contagranaFile.parseInt();
      contagranaFile.close();
    }
  }
  return contagrana;
}

void contagrana_data(uint32_t data){
  if (SPIFFS.begin(true))
  {
    File contagranaFile = SPIFFS.open("/contagrana.txt", "w");
    if (contagranaFile)
    {
      contagranaFile.print(data);
      contagranaFile.close();
    }
  }
}


uint32_t read_contaobj(){
uint32_t contaobj = 0;
  if (SPIFFS.begin(true))
  {
    File contaobjFile = SPIFFS.open("/contaobj.txt", "r");
    if (contaobjFile)
    {
      contaobj = contaobjFile.parseInt();
      contaobjFile.close();
    }
  }
  return contaobj;
}

void contaobj_data(uint32_t data){
  if (SPIFFS.begin(true))
  {
    File contaobjFile = SPIFFS.open("/contaobj.txt", "w");
    if (contaobjFile)
    {
      contaobjFile.print(data);
      contaobjFile.close();
    }
  }
}



uint32_t read_nvs_data(){
uint32_t last_id = 0;
  if (SPIFFS.begin(true))
  {
    File nvsFile = SPIFFS.open("/nvs.txt", "r");
    if (nvsFile)
    {
      last_id = nvsFile.parseInt();
      nvsFile.close();
    }
  }
  return last_id;
}

void save_nvs_data(uint32_t data){
  if (SPIFFS.begin(true))
  {
    File nvsFile = SPIFFS.open("/nvs.txt", "w");
    if (nvsFile)
    {
      nvsFile.print(data);
      nvsFile.close();
    }
  }
}



//################################ Função para salvar no arquivo txt
void savefile(const String &estado, const String &mensagem) {
  File file = SPIFFS.open("/estado.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo.");
    return;
  }

  file.println(estado);
  file.println(mensagem);
  file.close();
}

// Função para carregar o SSID e a senha do SPIFFS
void  loadfile(String &estado, String &mensagem) {
  File file = SPIFFS.open("/estado.txt", "r");
  if (!file) {
    Serial.println("Arquivo não encontrado.");
    return;
  }

  estado = file.readStringUntil('\n');
  mensagem = file.readStringUntil('\n');

  file.close();
}

  void deletefile() {
    String estado0 ="";
    String mensagem0 ="";
    savefile(estado0,mensagem0);      
    }

    
void acionabraco() {
  
        digitalWrite(relayPin, HIGH);
        delay(1000);
        digitalWrite(relayPin2, LOW);
        delay(5000);
        digitalWrite(relayPin, LOW);
        delay(1000);
        digitalWrite(relayPin2, HIGH);
 
  
  }



void print_qrcode(const uint8_t qrcode[]) {
  int size = qrcodegen_getSize(qrcode);
  char border = 4;
  char module_size = 4;
  uint16_t color;
  int xOffset = 6;
  int yOffset = 86;

  for (int y = -border; y < size + border; y++) {
    for (int x = -border; x < size + border; x++) {
      color = qrcodegen_getModule(qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      int pixelX = (x + border) * module_size + xOffset;
      int pixelY = (y + border) * module_size + yOffset;

      for (int i = 0; i < module_size; i++) {
        for (int j = 0; j < module_size; j++) {
          tft.drawPixel(pixelX + j, pixelY + i, color);
        }
      }
    }
  }
}

void qrcodefun() {

  Serial.println("QR CODE: " + payloadx);
  Serial.println("Gerando QR Code...");

  uint8_t* qrcode = (uint8_t*)malloc(qrcodegen_BUFFER_LEN_MAX);
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

  bool ok = qrcodegen_encodeText(payloadx.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

  if (ok) {
    print_qrcode(qrcode);
  }

  free(qrcode); // Liberar a memória alocada dinamicamente
  Serial.println("QR Code gerado e Impresso");
}


void touchInterrupt() {


 // Leia as coordenadas do toque
    TS_Point p = ts.getPoint();

    // Verifique se o toque está dentro das coordenadas aceitáveis
  if (p.z > 10 && p.z < 4000) {
      int16_t x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
      int16_t y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

      // Limpe a tela
      tft.fillScreen(TFT_BLACK);

      // Exiba as coordenadas do toque
      tft.setCursor(100, 100);
      tft.setTextColor(TFT_WHITE);
      tft.setTextSize(6);
      tft.print("Toque em: ");
      tft.print(x);
      tft.print(", ");
      tft.println(y);

    Serial.print("Coordenadas de toque: ");
 // Imprima as coordenadas do toque
    Serial.print("X: ");
    Serial.print(p.x);
    Serial.print(", Y: ");
    Serial.println(p.y);
    delay(1000);
      
    }




    
  }
