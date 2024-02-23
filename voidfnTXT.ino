

//########################################TELEMETRIA
     void telemetria(){
      HTTPClient https;
      obterHoraEDataAtual();

 String CHAVE2PHP = url;

  Serial.println("INICIA ENVIO TELEMETRIA...\n");
 // if (https.begin(client,CHAVE2PHP)) {
  if (https.begin(CHAVE2PHP)) {
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
  // Construa o corpo da solicita                  o POST
  String postData = "mac_value=" + WiFi.macAddress();
  postData += "&iplocal=" + WiFi.localIP().toString();
  
// Verifica se os sensores devem ser lidos ou não
#if (SENSORES == 1)

if (aht.begin()) {
    
   Serial.println("AHT inicializado com sucesso!");
     Serial.println("DADOS SENSORES ");
  Serial.println(readDHTTemperature());
  Serial.println(readDHTHumidity());
  Serial.println(readDHTPressao());
    // Se os sensores devem ser lidos, adquire suas leituras
  postData += "&temperatura=" + readDHTTemperature();
  postData += "&umidade=" + readDHTHumidity();
  postData += "&pressao=" + readDHTPressao();
  postData += "&ph=7";

  }else{
    Serial.println("Falha ao inicializar o sensor AHT!");
      // Se os sensores não devem ser lidos, define valores padrão como 0
  postData += "&temperatura=0";
  postData += "&umidade=0";
  postData += "&pressao=0";
  postData += "&ph=0";

}


#else
  // Se os sensores não devem ser lidos, define valores padrão como 0
  postData += "&temperatura=0";
  postData += "&umidade=0";
  postData += "&pressao=0";
  postData += "&ph=0";
#endif

  postData += "&wifilocal=" + WiFi.SSID();
  postData += "&horalocal=" + horaAtual;
  postData += "&datalocal=" + dataAtual;
  postData += "&nomedobot=" + nomedobot;
  postData += "&geo=" + geo;
  postData += "&usuarios=" + usuario;

    int httpCode = https.POST(postData);
      Serial.println(postData);
      Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been sent and Server response header has been handled
      Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
      String payload = https.getString();
      Serial.println("RESPOSTA DO SERVDIOR: ");
      Serial.println(payload);
    }

Serial.println("Hora atual: " + horaAtual);
Serial.println("Data atual: " + dataAtual);

    Serial.println("");
    Serial.println(F("Desconectando."));
    https.end();

    //digitalWrite(RGB_BUILTIN, LOW);
  } else {
    Serial.println(F("Falha na conex         o"));
    Serial.println();
    //neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Red
  }
      
}






// Fun                  o para carregar o SSID e a senha do SPIFFS




//################################ Fun                  o para salvar no arquivo txt
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

// Fun                  o para carregar o SSID e a senha do SPIFFS
void  loadfile(String &estado, String &mensagem) {
  File file = SPIFFS.open("/estado.txt", "r");
  if (!file) {
    Serial.println("Arquivo nao encontrado.");
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

    
