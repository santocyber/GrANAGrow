const char* serverUrl = "http://santocyber.helioho.st/granagw/telegrambot.php";
const char* serverUrl2 = "http://santocyber.helioho.st/granagw/lermsgtg.php";
const char* apiUrl = "http://santocyber.helioho.st/granagw/upload.php";
const char* serverName = "http://santocyber.helioho.st/granagw/respondetg.php";

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


void checkMessages() {
 
}


void sendResponse(String response, int messageId) {

}


#if (CAMERA == 1)




// Função para enviar a foto para a API
void sendPhotoToAPI() {
  // Captura da foto (mantenha o código de captura da foto aqui)

  // Envio da foto para a API PHP
  HTTPClient http;
  http.begin(apiUrl);
  http.addHeader("Content-Type", "image/jpeg");
  int httpResponseCode = http.POST((uint8_t*)fb->buf, fb->len);
  
  if (httpResponseCode > 0) {
    Serial.print("Resposta da API: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Erro ao enviar a foto. Código de erro: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  esp_camera_fb_return(fb);
}

 #endif       
