 #if (TELA == 1)

 
 
 uint16_t x, y;
 uint8_t z;






//#########################################VOIDS TOQUE


void toque() {
   //     Serial.println("STATETELA");   
    //    Serial.println(statetela);   

 // Verifica se a string é igual a "donate"
  
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
  
  } 

#if (RELES == 1)

   if (statetela == "botaorelay") {
        Serial.println("FUNCAO botaorelay");   
        botaorelay();      
  } 

#endif
  
  if (statetela == "telegram") {
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
            statetela = "clima";
            break;
        case 3:
            statetela = "site";
            break;
        case 4:
            statetela = "news";
            break;
        case 5:
            statetela = "hora";
            break;
        case 6:
            statetela = "scrolling";
            break;
        case 7:
            statetela = "crypto";
            break; 
        case 8:
            statetela = "bolsa";
            break;
        case 9:
            statetela = "mensagem";
            break;
        case 10:
            statetela = "horax";
            break; 
        case 11:
            statetela = "horiz";
            break; 
        case 12:
            statetela = "doa";
            break;  
         case 13:
            statetela = "donate";
            break;  
         case 14:
            statetela = "menu";
            break;  
         case 15:
            statetela = "lermsgtg";
            break;  
         case 16:
            statetela = "olhos";
            break;  
         case 17:
            statetela = "ph";
            break;  
        case 18:
            statetela = "vert";
            break;
        case 19:
            statetela = "telegram";
            break;
            delay(2000);

    }
  }
  
  }
 



void verificasqltela(){
      Serial.println("INICIA TELA STATUS SPIFF");

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

     if (mensagemstatus == "13") {
          Serial.println("BOTAO RELAY pelo SQL");
          functionExecuted = false;  // Marca a função como executada
          #if (RELES == 1)

          drawButtonsrelay();

          #endif
          delay(300);
          statetela = "botaorelay";
          boolbotao = true;

          touchCount = 0;  // Redefine o contador se exceder 4

    }
    
    if (mensagemstatus == "14") {
          Serial.println("FOTO pelo SQL");
          functionExecuted = false;  // Marca a função como executada
          statetela = "fototft";
          touchCount = 0;  // Redefine o contador se exceder 4

    }
        if (mensagemstatus == "15") {
          Serial.println("news pelo SQL");
          functionExecuted = false;  // Marca a função como executada
          statetela = "news";
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

#if (RELES == 1)


// Definindo as áreas dos botões
// Definindo as dimensões da tela
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Definindo o número de botões e o espaçamento entre eles
#define NUM_BUTTONS 3
#define BUTTON_SPACING 20

// Definindo as dimensões dos botões
#define BUTTON_W 200
#define BUTTON_H 80

// Calculando as posições dos botões
#define BUTTON1_X ((SCREEN_WIDTH - BUTTON_W) / 2)
#define BUTTON1_Y (BUTTON_SPACING)
#define BUTTON2_X ((SCREEN_WIDTH - BUTTON_W) / 2)
#define BUTTON2_Y (BUTTON1_Y + BUTTON_H + BUTTON_SPACING)
#define BUTTON3_X ((SCREEN_WIDTH - BUTTON_W) / 2)
#define BUTTON3_Y (BUTTON2_Y + BUTTON_H + BUTTON_SPACING)


void botaorelay() {
  while (boolbotao) {
    // Verificando se a tela foi tocada
    if (tft.getTouch(&x, &y)) {
      // Verificando se o toque foi dentro da área do botão 1
      if (x > BUTTON1_X && x < (BUTTON1_X + BUTTON_W) && y > BUTTON1_Y && y < (BUTTON1_Y + BUTTON_H)) {
        // Botão de sair não altera relé, mas pode ser usado para sair do loop
        Serial.println("BOTAO RELAY1");
        toggleRelay(RELAY1_PIN, BUTTON3_X, BUTTON3_Y);  
        delay(200); // Debounce
      }
      // Verificando se o toque foi dentro da área do botão 2
      if (x > BUTTON2_X && x < (BUTTON2_X + BUTTON_W) && y > BUTTON2_Y && y < (BUTTON2_Y + BUTTON_H)) {
        toggleRelay(RELAY2_PIN, BUTTON2_X, BUTTON2_Y);
        Serial.println("BOTAO RELAY2");
        delay(200); // Debounce
      }
      // Verificando se o toque foi dentro da área do botão 3
      if (x > BUTTON3_X && x < (BUTTON3_X + BUTTON_W) && y > BUTTON3_Y && y < (BUTTON3_Y + BUTTON_H)) {
        Serial.println("BOTAO SAIR");
         boolbotao = false;
        delay(200); // Debounce
      }
    }
  }
}

void drawButtonsrelay() {
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(rotate); // Ajusta a rotação do display, se necessário

  // Desenhando o botão de sair
  tft.fillRect(BUTTON1_X, BUTTON1_Y, BUTTON_W, BUTTON_H, TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("SAIR", BUTTON1_X + 10, BUTTON1_Y + 10, 2);

  // Desenhando os botões dos relés
  drawRelayButton(RELAY2_PIN, BUTTON2_X, BUTTON2_Y);
  drawRelayButton(RELAY1_PIN, BUTTON3_X, BUTTON3_Y);
}

void drawRelayButton(int relayPin, int x, int y) {
  int state = digitalRead(relayPin);
  if (state == HIGH) { // Assume que HIGH é desligado
    tft.fillRect(x, y, BUTTON_W, BUTTON_H, TFT_GREEN);
    tft.setTextColor(TFT_RED);
    tft.drawString("LIGADO", x + 10, y + 10, 2);
  } else {
    tft.fillRect(x, y, BUTTON_W, BUTTON_H, TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.drawString("OFF", x + 10, y + 10, 2);

  }
}

void toggleRelay(int relayPin, int x, int y) {
  int state = digitalRead(relayPin);
  digitalWrite(relayPin, !state); // Alterna o estado do relé
  drawRelayButton(relayPin, x, y); // Redesenha o botão com o novo estado

if (estado == "liga") {
    estado = "desliga";
  } else {
    estado = "liga";
  }
  savefile(estado, telastatus, mensagemstatus, mensagem);

  Serial.println("ESTADO RELE");
  Serial.println(state);
  Serial.println(relayPin);
  Serial.println(estado);

}
#endif
  


#endif
