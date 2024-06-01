const char* otaServer = "http://santocyber.helioho.st/granagw/uploads/update.bin"; // Endereço IP do servidor PHP
const char* otaconfirm = "http://santocyber.helioho.st/granagw/processar_mensagem90.php"; // Endereço IP do servidor PHP
const int otaPort = 80; // Porta do servidor PHP


#include <Update.h> // Incluindo a biblioteca Update.h


void updateFirmware() {
  Serial.println("Iniciando atualização de firmware...");
  #if (TELA == 1)

 tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0); // Set cursor at top left of screen

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("INICIANDO UPDATE DO GrANA robot");
  tft.setTextColor(TFT_RED, TFT_BLACK); // Change the font colour and the background colour
  tft.println("VERSAO ANTIGA");
  tft.println(GRANAVERSION);
#endif
  
  HTTPClient http;
  http.begin(otaServer); // Combina o endereço IP do servidor e o caminho do arquivo OTA

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    WiFiClient *stream = http.getStreamPtr();
    if (Update.begin(http.getSize(), U_FLASH)) { // Especifica o tipo de memória onde o firmware será gravado (neste caso, U_FLASH)
      Serial.println("Gravando firmware...");
      #if (TELA == 1)

 tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0); // Set cursor at top left of screen

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("GRAVANDO FIRMWARE");
  tft.setTextColor(TFT_RED, TFT_BLACK); // Change the font colour and the background colour
  tft.println("VERSAO ANTIGA");
  tft.println(GRANAVERSION);
  #endif
      size_t written = Update.writeStream(*stream);
      if (written == http.getSize()) {
        Serial.println("Atualização bem-sucedida!");
      } else {
        Serial.println("Falha ao escrever o firmware...");
      }

      if (Update.end(true)) {
        Serial.println("Firmware atualizado com sucesso!");
        Serial.println(GRANAVERSION);
        #if (TELA == 1)

        tft.setTextWrap(true, true); // Wrap on width and height

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0); // Set cursor at top left of screen

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("FIRMWARE ATUALIZADO COM SUCESSO");
  tft.setTextColor(TFT_RED, TFT_BLACK); // Change the font colour and the background colour
  tft.println("VERSAO ANTIGA");
  tft.println(GRANAVERSION);
  tft.println("REINICIE O ROBO");
#endif
  delay(5000);
          touchCount++; 

      } else {
        Serial.println("Erro ao finalizar a atualização...");
      }
    } else {
      Serial.println("Falha ao iniciar a atualização...");
    }
  } else {
    Serial.printf("Falha ao conectar ao servidor HTTP, código de erro: %d\n", httpCode);

  }

  http.end();
}





void otaconfirma() {
    HTTPClient http; // Cria um objeto HTTPClient
    http.begin(otaconfirm); // Inicializa a conexão HTTP    
    // Especifica o tipo de conteúdo como application/x-www-form-urlencoded
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
    // Cria o corpo da solicitação POST
    String postData = "mac=" + String(WiFi.macAddress()) + "&username=" + String(usuario);
    int httpResponseCode = http.POST(postData); // Envia a solicitação HTTP POST 
    if (httpResponseCode > 0) { // Verifica o código de resposta HTTP
      String response = http.getString(); // Obtém a resposta como string
      Serial.println(httpResponseCode); // Imprime o código de resposta
      Serial.println(response); // Imprime a resposta
    } else {
      Serial.print("Erro na solicitação HTTP: ");
      Serial.println(httpResponseCode);
    }   
    http.end(); // Finaliza a conexão HTTP
}




    
