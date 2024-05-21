//########################################TELEMETRIA
void telemetria(){
  HTTPClient https;
  obterHoraEDataAtual();


  Serial.println("INICIA ENVIO TELEMETRIA...\n");
  if (https.begin(url)) {
    
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Construa o corpo da solicitação POST
    String postData = "mac=" + WiFi.macAddress();
    postData += "&iplocal=" + WiFi.localIP().toString();
  
    // Verifica se os sensores devem ser lidos ou não
    #if (SENSORES == 1)


      Serial.println(ph());
      Serial.println(analogRead(phpin));


    
    if (aht.begin()) {
      Serial.println("AHT inicializado com sucesso!");
      Serial.println("DADOS SENSORES ");
      Serial.println(readDHTTemperature());
      Serial.println(readDHTHumidity());
      Serial.println(readDHTPressao());
    
      // Se os sensores devem ser lidos, adquire suas leituras
      postData += "&temperatura=" + String(readDHTTemperature());
      postData += "&umidade=" + String(readDHTHumidity());
      postData += "&pressao=" + String(readDHTPressao());
      postData += "&ph=" + String(ph(), 3);
    } else {
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
    postData += "&versao=" + GRANAVERSION;
    postData += "&ping=" + String(Ping.averageTime());

    int httpCode = https.POST(postData);
    Serial.println(postData);
    Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been sent and Server response header has been handled
      String payload = https.getString();
      Serial.println("RESPOSTA DO SERVIDOR: ");
      Serial.println(payload);
    }

    Serial.println("Hora atual: " + horaAtual);
    Serial.println("Data atual: " + dataAtual);

    Serial.println("");
    Serial.println(F("Desconectando."));
    https.end();

    //digitalWrite(RGB_BUILTIN, LOW);
  } else {
    Serial.println(F("Falha na conexão"));
    Serial.println();
    //neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Red
  }
}

//##########################################VERIFICA FUNCOES PENDENTES NO SQL
void verifyActionAndExecute() {
  Serial.println("INICIA VERIFICA EXECUTA");


  HTTPClient https;

  if (https.begin(urlverifica)) {
  
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String postData = "mac=" + WiFi.macAddress();
    
    int httpCode = https.POST(postData);
    Serial.println(postData);
    Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

  if (httpCode > 0) {
    String payload = https.getString();
    Serial.println(payload);

    JsonDocument doc;
    deserializeJson(doc, payload);

    String acoes = doc["acoes"].as<String>();
    String estado = doc["estado"].as<String>();
    String mensagem = doc["mensagem"].as<String>();
    String telastatus = doc["telastatus"].as<String>();
    String mensagemstatus = doc["mensagemstatus"].as<String>();
    String hrliga = doc["hrliga"].as<String>();
    String hrdesliga = doc["hrdesliga"].as<String>();
    String timerautomatico = doc["timerautomatico"].as<String>();
    String timerfoto = doc["timerfoto"].as<String>();
    String timerfotostatus = doc["timerfotostatus"].as<String>();

    Serial.println("SALVANDO NA ROM");
    Serial.println(estado);
    Serial.println(acoes);
    Serial.println(mensagem);
    Serial.println(telastatus);
    Serial.println(mensagemstatus);
    Serial.println(hrliga);
    Serial.println(hrdesliga);
    Serial.println(timerautomatico);
    Serial.println(timerfotostatus);
    Serial.println(timerfoto);

    savefile(estado, telastatus, mensagemstatus, mensagem);
    saveTime(hrliga, hrdesliga, timerautomatico, timerfoto, timerfotostatus);
    Serial.println("LENDO ROM");
    acoes.trim();
    estado.trim();
    mensagem.trim();
    telastatus.trim();
    mensagemstatus.trim();
    hrliga.trim();
    hrdesliga.trim();



    
  } else {
    Serial.println("Falha na verifica executa: " + WiFi.macAddress());
  }
  }else{
    
        Serial.println("erro sem conexao");
}
  https.end();
}
