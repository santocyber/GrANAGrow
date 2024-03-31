
const char NO_KEY = '\0'; // Definindo NO_KEY como um caractere nulo
const int MAX_NETWORKS = 3; // Definir o máximo de redes para exibir na tela

const uint16_t MIN_PRESSURE = 200;
const uint16_t MAX_PRESSURE = 4000;
uint16_t x, y;
uint8_t z;

int scan() {
    // Scan for Wi-Fi networks
    int n = WiFi.scanNetworks();
    return n;
}


void scanWiFiNetworks() {
   int n = scan();
       delay(500); // Atualizar a lista a cada 5 segundos
   tft.init();
   tft.setRotation(3);
   tft.setCursor(0, 0); // Set cursor at top left of screen
   tft.fillScreen(TFT_BLACK);
   tft.setTextColor(TFT_WHITE, TFT_BLACK);
   tft.setTextSize(6);
   tft.println("Robot GrANA");
   tft.setTextColor(TFT_RED, TFT_BLACK);
   tft.setTextSize(4);
   tft.println("CONFIGURE O WIFI!!!");
   tft.setTextColor(TFT_WHITE, TFT_BLACK);
   tft.setTextSize(2);
   tft.println("CONECTE-se ao AP GrANAConfig");
   tft.println("ACESSE O SITE 192.168.4.1");
   tft.setTextColor(TFT_YELLOW, TFT_BLACK);
   tft.setTextSize(1);
   tft.print("Internet encontradas: ");
   tft.print(n);
   tft.println("");



    for (int i = 0; i < min(n, MAX_NETWORKS); i++) {
    tft.println("");
    tft.setTextSize(3);
    tft.print(i + 1);
    tft.print(".");
    tft.print(WiFi.SSID(i));
    tft.print("-RSSI:");
    tft.println(WiFi.RSSI(i));
    Serial.println(WiFi.SSID(i));


  }


      // Desenhar botão "Escanear Novamente"
    tft.setTextSize(2);
    int buttonWidth = 200;
    int buttonHeight = 40;
    int buttonX = tft.width() - buttonWidth - 50; // Ajuste para o canto inferior direito
    int buttonY = tft.height() - buttonHeight - 10;
    tft.drawRect(buttonX, buttonY, buttonWidth + 50, buttonHeight, TFT_GREEN);
    tft.setCursor(buttonX + 20, buttonY + 10); // Ajustar posição do texto do botão
    tft.println("Escanear Novamente");


     // Aguardar toque na tela para selecionar o Wi-Fi ou escanear novamente
  // Aguardar toque na tela para selecionar o Wi-Fi ou escanear novamente
  unsigned long timeoutMillis = 30000; // Tempo limite de espera em milissegundos (30 segundos)
  unsigned long startTime = millis(); // Obter o tempo inicial
       while (true) {
        getKeyFromTouch(&x, &y, &z);

         // Verificar se o tempo limite de espera foi atingido
    if (millis() - startTime >= timeoutMillis) {
      break; // Sair do loop após o tempo limite
    }
    }

}



void getKeyFromTouch(uint16_t* x, uint16_t* y, uint8_t* z) {
    // Verifica se houve toque
    if (tft.getTouchRawZ() > 500) {
        delay(100); // Atualizar a lista a cada 5 segundos

        tft.getTouchRaw(x, y);
        *z = tft.getTouchRawZ();
        
        Serial.println("toque wifi");
        Serial.print("x: ");
        Serial.println(*x);
        Serial.print("y: ");
        Serial.println(*y);       
        Serial.print("z: ");
        Serial.println(*z);


 if (*x >= 1000 && *x <= 3200 && *y >= 2100 && *y <=3700 ) {
                // Botão "Escanear Novamente" pressionado, então escaneia novamente
                Serial.println("Escanear Novamente pressionado");
                scanWiFiNetworks();
                return;
            }
 if (*x >= 1600 && *x <= 1700 && *y >= 200 && *y <=1500 ) {
                Serial.println("ssid 1 selecionado");
                ssid = WiFi.SSID(0);
                Serial.println(ssid);
                saveWifiCredentials(ssid, password.c_str(), nomedobot.c_str(), geo.c_str(), usuario.c_str());
                enterPassword();

                return;
            }


        
    } else {
        *z = 0; // Define z como 0 se não houver toque
    }
}




  
char keys[5][11] = {
    {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '<'}, 
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '!'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '$', '%'},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', '.', '_', '*', '@'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '?', '&', '#'}
};

char key = keys[5][11];


String inputText = ""; // Variável para armazenar o texto digitado


void enterPassword() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.println("Digite a senha:");
  tft.println("Para o SSID:");
  tft.println(ssid);
  tft.println("E aperte #");


  delay(2000);
  uint8_t z; // Declaramos z fora do loop para obter a pressão do toque
     drawKeyboard();

  while (true) {
    getKeyFromTouch2(&x, &y, &z); // Obter toque
    if (z > MIN_PRESSURE) {
      // Desenhar teclado na tela
      if (key == '#') {
        break;
        return;
      } 
    }
  }
}







void getKeyFromTouch2(uint16_t* x, uint16_t* y, uint8_t* z) {
    // Verifica se houve toque
    if (tft.getTouchRawZ() > 500) {
        tft.getTouchRaw(x, y);
        *z = tft.getTouchRawZ();
        delay(300);

        Serial.println("Toque detectado:");
        Serial.print("x: ");
        Serial.println(*x);
        Serial.print("y: ");
        Serial.println(*y);
        Serial.print("z: ");
        Serial.println(*z);

int rowIndex = map(*x, 444,3555, 0, 5);
int colIndex = map(*y, 444, 3666, 0, 11);
colIndex = constrain(colIndex, 0, 10);
rowIndex = constrain(rowIndex, 0, 4);


        char key = keys[rowIndex][colIndex];

        // Adicionar o caractere ao texto digitado ou executar ação conforme o botão pressionado
        if (key != ' ') {
               Serial.println("key");
               Serial.println(key);
               Serial.println(rowIndex);
               Serial.println(colIndex);


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
                   password = inputText;
                   Serial.println(password);

                   inputText = "";
                   key = NO_KEY;
                   saveWifiCredentials(ssid, password, nomedobot.c_str(), geo.c_str(), usuario);

                     tft.fillScreen(TFT_BLACK);
                     tft.setCursor(0, 0); 
                     tft.setTextSize(4);
                     tft.setTextColor(TFT_PINK);
                     tft.print("Senha:");
                     tft.println(password);
                     tft.println("Salvo com sucesso!");
                     delay(4000);
                     enterUsuario();
                     return;


                   
                }
                
            drawKeyboard(); // Redesenhar o teclado para atualizar o texto digitado na tela
        }
    } else {
        *z = 0; // Define z como 0 se não houver toque
    }
}

















void enterUsuario() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.println("Digite o usuario:");
  tft.println("E aperte #");


  delay(2000);

  uint8_t z; // Declaramos z fora do loop para obter a pressão do toque

     drawKeyboard();

  while (true) {
    getKeyFromTouch3(&x, &y, &z); // Obter toque
    if (z > MIN_PRESSURE) {
      // Desenhar teclado na tela
      if (key == '#') {
        break;
        return;
        
      } 
    }
  }
}






void getKeyFromTouch3(uint16_t* x, uint16_t* y, uint8_t* z) {
    // Verifica se houve toque
    if (tft.getTouchRawZ() > 500) {
        tft.getTouchRaw(x, y);
        *z = tft.getTouchRawZ();
        delay(300);

  
int rowIndex = map(*x, 444,3555, 0, 5);
int colIndex = map(*y, 444, 3666, 0, 11);
colIndex = constrain(colIndex, 0, 10);
rowIndex = constrain(rowIndex, 0, 4);


        char key = keys[rowIndex][colIndex];

        // Adicionar o caractere ao texto digitado ou executar ação conforme o botão pressionado
        if (key != ' ') {
               Serial.println("key");
               Serial.println(key);
               Serial.println(rowIndex);
               Serial.println(colIndex);


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
                   usuario = inputText;
                   Serial.println(usuario);

                   saveWifiCredentials(ssid, password, nomedobot.c_str(), geo.c_str(), usuario);
                   inputText = "";
                   key = NO_KEY;

                     tft.fillScreen(TFT_BLACK);
                     tft.setCursor(0, 0); 
                     tft.setTextSize(4);
                     tft.setTextColor(TFT_PINK);
                     tft.print("Usuario:");
                     tft.println(usuario);
                     tft.println("Salvo com sucesso!");
                     delay(4000);
                    menu();

                   return;

                }
                
            drawKeyboard(); // Redesenhar o teclado para atualizar o texto digitado na tela
        }
    } else {
        *z = 0; // Define z como 0 se não houver toque
    }
}


void drawKeyboard() {
    // Limpar tela
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0); // Posição para exibir o texto na parte inferior da tela
  // Desenhar teclado na tela
    int keyWidth = tft.width() / 11; // Número de colunas no teclado
    int keyHeight = tft.height() / 5; // Número de linhas no teclado



    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 11; col++) {
            char key = keys[row][col];
            int x = col * keyWidth;
            int y = row * keyHeight;
            tft.fillRect(x, y, keyWidth, keyHeight, TFT_WHITE);
            tft.setCursor(x + keyWidth / 4, y + keyHeight / 4);
            tft.setTextColor(TFT_BLACK);
            tft.setTextSize(6);
            tft.print(key);
        }
    }

    // Exibir o texto digitado na tela
    tft.setCursor(5, tft.height() - 40); // Posição para exibir o texto na parte inferior da tela
    tft.setTextSize(4);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print(":");
    tft.print(inputText);
}
