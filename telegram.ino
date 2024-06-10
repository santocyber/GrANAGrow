const char* serverUrl = "http://santocyber.helioho.st/granagw/telegrambot.php";
const char* serverUrl2 = "http://santocyber.helioho.st/granagw/lermsgtg.php";
const char* uploadUrl = "http://santocyber.helioho.st/granagw/enviafototg.php";
const char* serverName = "http://santocyber.helioho.st/granagw/respondetg.php";
const char* apicomando = "http://santocyber.helioho.st/granagw/api_comandotg.php";

void sendPostRequest() {
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
 // http.addHeader("Content-Type", "text/plain");
  int httpResponseCode = http.POST(msgtg);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}





#if (TELA == 1)

void lermsgtg() {

  // Fazer solicitação ao servidor PHP para obter mensagens
  HTTPClient http;
  http.begin(serverUrl2);
  int httpResponseCode = http.GET();
  
  if (httpResponseCode == 200) {
    String response = http.getString();
    displayMessage(response);
  } else {
    Serial.println("Error on HTTP request");
      tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
      tft.println("Error on HTTP request");

  }
  http.end();

  delay(500); // Ajuste este valor conforme necessário para a taxa de atualização
}




void telegrammsg() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.println("Digite a mensagem:");
  tft.println("E aperte #");


  delay(3000);

  uint8_t z; // Declaramos z fora do loop para obter a pressão do toque

     drawKeyboard();


}


#endif

#if (CAMERA == 1)
void sendPhotoToAPI() {
    Serial.println("FUNCAO ENVIAR FOTO");



    nomedobot.trim();
    usuario.trim();

    // Constrói a URL com os parâmetros GET
    String urlWithParams = uploadUrl;
    urlWithParams += "?mac=" + WiFi.macAddress();
    urlWithParams += "&nomedobot=" + nomedobot;
    urlWithParams += "&usuario=" + usuario;
    urlWithParams += "&chat_id=" + chat_id;

    Serial.println("URL UPLOAD TG");
    Serial.println(urlWithParams);

    // Limpa o buffer da câmera antes de enviar a foto para a API
    esp_camera_fb_return(fb);

        // Captura da foto
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Erro ao capturar a foto");
        return;
    }
    
delay(400);

        // Limpa o buffer da câmera antes de enviar a foto para a API
    esp_camera_fb_return(fb);

        // Captura da foto
    esp_camera_fb_get();
    if (!fb) {
        Serial.println("Erro ao capturar a foto");
        return;
    }

    // Exibe o tamanho da foto capturada
    Serial.print("Tamanho da foto: ");
    Serial.println(fb->len);
    

    // Envio da foto para a API PHP
    HTTPClient http;
    http.begin(urlWithParams); // URL com parâmetros GET
    http.addHeader("Content-Type", "image/jpeg");

    Serial.println("Enviando foto para a API TG...");
    

    int httpResponseCode = http.POST((uint8_t*)fb->buf, fb->len);

    if (httpResponseCode > 0) {
        Serial.print("Resposta da API: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.print("Resposta da mensagem: ");
        Serial.println(response);
    } else {
        Serial.print("Erro ao enviar a foto. Código de erro: ");
        Serial.println(httpResponseCode);
        Serial.print("Motivo: ");
        Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    // Limpa o buffer da câmera após o envio da foto para a API
    esp_camera_fb_return(fb);
    http.end();
}


 #endif       
String ultimoComando = "";
void recebemsgtg() {
    String comando = getLastCommand();
       comando.toLowerCase();

    if (comando != ultimoComando) {
        // Normalize o comando para minúsculas

        // Use uma estrutura de dados mais eficiente para comparar comandos
        if (comando == "foto" || comando == "/foto") {
          #if (CAMERA == 1)
            sendPhotoToAPI();
            #endif
        } 
        
        if (comando == "oi") {
            Serial.println("Comando OI");
        }

        // Atualiza o último comando processado
        ultimoComando = comando;
    } else {
        Serial.println(chat_id);
        Serial.println(comando);
        Serial.println("Comando já processado.");
    }
}


// Função para obter o último comando do servidor
String getLastCommand() {
    Serial.println("LENDO ULTIMA MSG TG");

    HTTPClient http;
    http.begin(apicomando);
    int httpResponseCode = http.GET();
    String comando = "";
    if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.println(payload);

        // Processa a resposta JSON
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        comando = doc["comando"].as<String>();
        
        // Encontra a posição do caractere '@'
        int pos = comando.indexOf('@');
        if (pos != -1) { // Se o caractere '@' foi encontrado
            comando = comando.substring(0, pos); // Obtém a substring até '@'
        }
        
        chat_id = doc["chat_id"].as<String>();
    } else {
        Serial.print("Erro ao fazer a requisição. Código HTTP: ");
        Serial.println(httpResponseCode);
    }
    http.end();
    return comando;
}
