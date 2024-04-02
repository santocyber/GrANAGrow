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






void getKeyFromTouch4(uint16_t* x, uint16_t* y, uint8_t* z) {
    // Verifica se houve toque
    if (tft.getTouchRawZ() > 500) {
        contador = 0;

        tft.getTouchRaw(x, y);
        *z = tft.getTouchRawZ();
        delay(300);

  
int rowIndex = map(*x, 444,3555, 0, 5);
int colIndex = map(*y, 444, 3666, 0, 11);
rowIndex = constrain(rowIndex, 0, 5);
colIndex = constrain(colIndex, 0, 10);


        char key = keys[rowIndex][colIndex];

        // Adicionar o caractere ao texto digitado ou executar ação conforme o botão pressionado
        if (key != ' ') {
          
            // Ação para caracteres de texto
            if (rowIndex != 0 && inputText.length() < 20) { // Limite de 20 caracteres
                inputText += key;
            }
            // Ação para botões especiais
            else {
                if (key == '<') { // Botão de apagar
                    if (!inputText.isEmpty()) {
                        inputText.remove(inputText.length() - 1); // Remove o último caractere digitado
                    }
                    
                }

            }

            if (key == '#') { // Botão de cancelar
                  Serial.println("ENTER TECLADO VIRTUAL");
                  inputText.remove(inputText.length() - 1);
                   msgtg = inputText;
                   Serial.println(msgtg);
                   inputText = "";
                   key = NO_KEY;

                     tft.fillScreen(TFT_BLACK);
                     tft.setCursor(0, 0); 
                     tft.setTextSize(4);
                     tft.setTextColor(TFT_YELLOW);
                     tft.println("MENSAGEM ENVIADA AO TELEGRAM");
                     tft.println(msgtg);
                     sendPostRequest();

                     delay(4000);
                     statetela = "olhos";

                   return;

                }
                
            drawKeyboard(); // Redesenhar o teclado para atualizar o texto digitado na tela
        }
    } else {
        *z = 0; // Define z como 0 se não houver toque
    }
}

        
