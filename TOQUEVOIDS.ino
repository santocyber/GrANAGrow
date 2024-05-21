 #if (TELA == 1)

 
 
 uint16_t x, y;
 uint8_t z;

const String rss_feed_urls[2] = {
  "https://feeds.bbci.co.uk/portuguese/rss.xml",
  "https://rss.tecmundo.com.br/feed",
};





//#########################################VOIDS TOQUE


void toque() {
   //     Serial.println("STATETELA");   
    //    Serial.println(statetela);   

 // Verifica se a string é igual a "donate"
  if (statetela == "news") {
        checkButtonPress();
        Serial.println("FUNCAO NEWS");
        contador++;
        Serial.println(contador);

     if (contador >= 20) {
        statetela = "padrao";
        touchCount++; 
        contador = 0;

  }  
    
  }
  
  if (statetela == "donate") {
        Serial.println("FUNCAO DONATE");   
        
        touchEventHandlerpix();

        contador++;
        Serial.println(contador);

 
      if (contador >= 20) {
          boolpix = false;
          contadorpix = 0;
          functionExecuted = false;  // Marca a função como executada
          statetela = "olhos";
          olhos();
          touchCount++; 
          olhos();

          }  
  
  } if (statetela == "telegram") {
        Serial.println("FUNCAO TELEGRAM");

        getKeyFromTouch4(&x, &y, &z);

        contador++;
        Serial.println(contador);

     if (contador >= 20) {
        statetela = "lermsgtg";
        touchCount++; 
        lermsgtg();
        contador = 0;
          }  
 
  }else{
    // Verifica se houve toque
    if (tft.getTouchRawZ() > 400) {
        tft.getTouchRaw(&x, &y);
        z = tft.getTouchRawZ();
        
        Serial.println("Toque detectado:");
        Serial.print("x: ");
        Serial.println(x);
        Serial.print("y: ");
        Serial.println(y);
        Serial.print("z: ");
        Serial.println(z);
        Serial.println(tft.getTouch(&x, &y));

        //vTaskDelay(400); // Aguarde 100ms antes de verificar novamente o toque
        delay(1000);

                // Incrementa o contador de toques apenas quando o dedo é levantado da tela
            touchCount++; 
             
            functionExecuted = false;  // Marca a função como executada
            boolpix = true;

            Serial.println("Número de toques: " + String(touchCount));
            

    } 

           
            if (touchCount >= 19) {
                touchCount = 0;  // Redefine o contador se exceder 4
                Serial.println("O contador de toques foi redefinido.");
            }
        
    

  

        

    // Lógica para diferentes ações com base no número de toques
    switch (touchCount) {

        case 1:
            statetela = "clock";
            break;
        case 2:
            statetela = "hora";
            break;
        case 3:
            statetela = "clima";
            break;
        case 4:
            statetela = "ph";
            break;
        case 5:
            statetela = "news";
            break;
        case 6:
            statetela = "telegram";
            break;
        case 7:
            statetela = "lermsgtg";
            break; 
        case 8:
            statetela = "scrolling";
            break;
        case 9:
            statetela = "horiz";
            break;
        case 10:
            statetela = "horax";
            break; 
        case 11:
            statetela = "mensagem";
            break; 
        case 12:
            statetela = "site";
            break;  
         case 13:
            statetela = "vert";
            break;  
         case 14:
            statetela = "menu";
            break;  
         case 15:
            statetela = "bolsa";
            break;  
         case 16:
            statetela = "crypto";
            break;  
         case 17:
            statetela = "doa";
            break;  
        case 18:
            statetela = "donate";
            break;
        case 19:
            statetela = "olhos";
            break;
            delay(2000);

    }
  }
  
  }
 



void verificasqltela(){
      Serial.println("inicia mensagem tela sql");

      loadfile(estado, telastatus, mensagemstatus, mensagem);
      telastatus.trim();
      mensagemstatus.trim();
      mensagem.trim();

      



//########################################  STATUS ROTATION

   if (telastatus == "1") {
          Serial.println("Rotate 4");
          rotate = 1;
    } 
      
      if (telastatus == "2") {
          Serial.println("Rotate 2");
          rotate = 2;
    }

     if (telastatus == "3") {
          Serial.println("Rotate 3");
          rotate = 3;
    }   

    if (telastatus == "4") {
          Serial.println("Rotate 4");
          rotate = 4;
    }  
    
    if (telastatus == "5") {
          Serial.println("Rotate 5");
          rotate = 5;
    }
    
    if (telastatus == "6") {
          Serial.println("Rotate 6");
          rotate = 6;
    }  


    
//####################################### STATUS TELA


  
     //  functionExecuted = false;  // Marca a função como executada

    if (mensagemstatus == "0") {
      Serial.println("Nao faz nada pelo SQL");
    }

     if (mensagemstatus == "1") {
         Serial.println("Scrolling pelo SQL");
         Serial.println(mensagemstatus);
         functionExecuted = false; // Reseta a variável para próxima execução
         statetela = "scrolling";
         touchCount = 0;  // Redefine o contador se exceder 4


    } 
      
      if (mensagemstatus == "2") {
          Serial.println("Hora analog pelo SQL");
          Serial.println(mensagemstatus);
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "clock";
          touchCount = 0;  // Redefine o contador se exceder 4

    }

     if (mensagemstatus == "3") {
          Serial.println("Hora digital pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "hora";
          touchCount = 0;  // Redefine o contador se exceder 4
 
    }   

    if (mensagemstatus == "4") {
          Serial.println("Clima pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "clima";
          touchCount = 0;  // Redefine o contador se exceder 4

    }  
    
    if (mensagemstatus == "5") {
          Serial.println("Mensagem pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "mensagem";
          touchCount = 0;  // Redefine o contador se exceder 4
  
    } 
    
    if (mensagemstatus == "6") {
          Serial.println("Olho pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "olhos";
          touchCount = 0;  // Redefine o contador se exceder 4
   
    }
   
    if (mensagemstatus == "7") {
          Serial.println("BOLSA pelo SQL");
          statetela = "bolsa";
          touchCount = 0;  // Redefine o contador se exceder 4
   
    }

     if (mensagemstatus == "8") {
          Serial.println("QRcode site pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "site";
          touchCount = 0;  // Redefine o contador se exceder 4

    }
    
    if (mensagemstatus == "9") {
          Serial.println("QRcode doa pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "doa";
          touchCount = 0;  // Redefine o contador se exceder 4

    }
    
    if (mensagemstatus == "10") {
          Serial.println("CHAT pelo SQL");

          statetela = "chat";
          touchCount = 0;  // Redefine o contador se exceder 4

    }
        if (mensagemstatus == "11") {
          Serial.println("PH pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "ph";
          touchCount = 0;  // Redefine o contador se exceder 4

    }

        if (mensagemstatus == "12") {
          Serial.println("CRYPTO pelo SQL");

          statetela = "crypto";
          touchCount = 0;  // Redefine o contador se exceder 4

    }
    
      if (mensagemstatus == "90") {
          Serial.println("UPDATE OTA PELA WEB");
          statetela = "OTA";
          touchCount = 0;  // Redefine o contador se exceder 4
          mensagemstatus = "0";
          updateFirmware();
          otaconfirma();
          savefile(estado, telastatus, mensagemstatus, mensagem);

          delay(10000);
    }
    
      Serial.println(statetela);
      Serial.println(mensagemstatus);
 
  
  }



void touchEventHandlerpix() {
while (boolpix) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      contadorpix++; 
      Serial.println("contador pix ");
      Serial.println(contadorpix);

    }
 if (contadorpix == 20) {
          boolpix = false;
          contadorpix = 0;
          functionExecuted = false;  // Marca a função como executada
          statetela = "olhos";
          olhos();
          touchCount++; 
          touchCount = 0;  // Redefine o contador se exceder 4
          olhos();



 }

  
  if (tft.getTouch(&x, &y)) {
     contadorpix = 0;

    contador = 0;
    x = map(x, 0, tft.width(), tft.width(), 0);
    y = map(y, 0, tft.height(), tft.height(), 0);
    delay(100);

    checkButtons(x, y);
  }
}}

  
void checkButtonPress() {
      if (tft.getTouchRawZ() > 400) {
        contador = 0;

    tft.getTouchRaw(&x, &y);
    int buttonIndex = mapTouchToButton(x, y);
    if (buttonIndex != -1) {
      Serial.println(buttonIndex);
      fetchAndPrintRSSWithRetry(rss_feed_urls[buttonIndex]);
      functionExecuted = true;  // Marca a função como executada
      buttonsCreated = false;
      Executed = false;  // Marca a função como executada
                 // touchCount++; 
       buttonIndex = -1;



    }}
  
}



int mapTouchToButton(uint16_t x, uint16_t y) {
  // Definir as coordenadas máximas e mínimas dos eixos X e Y
  int minX = 444;
  int maxX = 3555;
  int minY = 444;
  int maxY = 3666;

  // Definir o número de colunas e linhas de botões
  int numCols = 2;
  int numRows = 1;

  // Mapeando as coordenadas X e Y para os índices de linha e coluna
  int colIndex = map(x, minX, maxX, 0, numCols); // 0 a numCols-1
  int rowIndex = map(y, minY, maxY, 0, numRows); // 0 a numRows-1

  // Restringindo os índices dentro dos limites
  colIndex = constrain(colIndex, 0, numCols - 1);
  rowIndex = constrain(rowIndex, 0, numRows - 1);

  // Calculando o índice do botão
  int buttonIndex = rowIndex * numCols + colIndex;

  // Adicionar mensagens de depuração para verificar os índices calculados
  Serial.println("XY");
  Serial.println(x);
  Serial.println(y);

  Serial.println("COLUNA ROW INDEX");
  Serial.println(colIndex);
  Serial.println(rowIndex);

  // Adicionar mensagem de depuração para verificar o índice do botão calculado
  Serial.println("button INDEX");
  Serial.println(buttonIndex);

  // Retornar o índice do botão
  return buttonIndex;
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

                     delay(2000);
                     lermsgtg();
                     return;

                }
                
            drawKeyboard(); // Redesenhar o teclado para atualizar o texto digitado na tela
        }
    } else {
        *z = 0; // Define z como 0 se não houver toque
    }
}


#endif
