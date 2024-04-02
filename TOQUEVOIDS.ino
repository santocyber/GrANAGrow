 uint16_t x, y;
 uint8_t z;

const String rss_feed_urls[2] = {
  "https://feeds.bbci.co.uk/portuguese/rss.xml",
  "https://rss.tecmundo.com.br/feed",
};





//#########################################VOIDS TOQUE


void toque() {

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
    
  }  if (statetela == "donate") {
        Serial.println("FUNCAO DONATE");   
  } if (statetela == "telegram") {
        Serial.println("FUNCAO TELEGRAM");
        contador++;
        Serial.println(contador);

     if (contador >= 20) {
        contador = 0;
        statetela = "padrao";
        touchCount++; 
        statetela = "menu";


        
  }  
        getKeyFromTouch4(&x, &y, &z); // Obter toque
 
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

        //vTaskDelay(400); // Aguarde 100ms antes de verificar novamente o toque
        delay(500);

                // Incrementa o contador de toques apenas quando o dedo é levantado da tela
            touchCount++; 
             
            functionExecuted = false;  // Marca a função como executada

            Serial.println("Número de toques: " + String(touchCount));
            

    } 

           
            if (touchCount > 16) {
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
            statetela = "mensagem";
            break;
        case 5:
            statetela = "news";
            break;
        case 6:
            statetela = "telegram";
            break;
        case 7:
            statetela = "menu";
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
            statetela = "ph";
            break; 
        case 12:
            statetela = "site";
            break;  
         case 13:
            statetela = "vert";
            break;  
         case 14:
            statetela = "doa";
            break;  
        case 15:
            statetela = "donate";
            break;
        case 16:
            statetela = "olhos";
            break;
            delay(1000);

    }
  }
  
  }
 



void verificasqltela(){
      Serial.println("inicia mensagem tela sql");

      loadfile(estado, mensagem, mensagemstatus);
      mensagemstatus.trim();
    
      Serial.println(statetela);
      Serial.println(mensagemstatus);
     //  functionExecuted = false;  // Marca a função como executada

    if (mensagemstatus == "0") {
      Serial.println("Nao faz nada pelo SQL");
    }

     if (mensagemstatus == "1") {
         Serial.println("Scrolling pelo SQL");
         Serial.println(mensagemstatus);
         Serial.println(mensagem);
         functionExecuted = false; // Reseta a variável para próxima execução
         statetela = "scrolling";
    } 
      
      if (mensagemstatus == "2") {
          Serial.println("Hora analog pelo SQL");
          Serial.println(mensagemstatus);
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "clock";
    }

     if (mensagemstatus == "3") {
          Serial.println("Hora digital pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "hora"; 
    }   

    if (mensagemstatus == "4") {
          Serial.println("Clima pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "clima";
    }  
    
    if (mensagemstatus == "5") {
          Serial.println("Mensagem pelo SQL");
          Serial.println(mensagem);
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "mensagem";  
    } 
    
    if (mensagemstatus == "6") {
          Serial.println("Olho pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "olhos";   
    }

     if (mensagemstatus == "7") {
          Serial.println("QRcode site pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "site";
    }
    
    if (mensagemstatus == "8") {
          Serial.println("QRcode doa pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "doa";
    }
    
    if (mensagemstatus == "9") {
          Serial.println("NEWS pelo SQL");
          functionExecuted = false; // Reseta a variável para próxima execução
          statetela = "news";
          buttonIndex = 1;
    }

    
  
 
  
  }




  
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
