const char* serverUrl = "http://santocyber.helioho.st/granagw/telegrambot.php";


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




#if (CAMERA == 1)


const char* apiUrl = "http://suaapi.com/upload.php";


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
