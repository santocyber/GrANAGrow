#if (TELA == 1)


//#define TFT_WIDTH 240
//#define TFT_HEIGHT 320
#define SCROLL_SPEED 150 // Altere para ajustar a velocidade de rolagem
#define SCROLL_DELAY 10 // Altere para ajustar o intervalo de rolagem


// Variáveis para o texto e sua posição
int textX = 0;
int textY;
int scrollSpeed = 60;

//clock

#define TFT_GREY 0x5AEB

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg=0, mdeg=0, hdeg=0;
uint16_t osx=120, osy=120, omx=120, omy=120, ohx=120, ohy=120;  // Saved H, M, S x & y coords
uint16_t x0=0, x1=0, yy0=0, yy1=0;
uint32_t targetTime = 0;                    // for next 1 second timeout

static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

bool initial = 1;

//uint8_t hh, mm, ss;



void tela(){
   unsigned long currentMillis = millis();


if (statetela == "menu" && !functionExecuted) {
    menu();
    functionExecuted = true;  // Marca a função como executada
 }

  if (statetela == "mensagem" && !functionExecuted) {
     Serial.println("modo mensagem");
     displayMessage(mensagem);
     functionExecuted = true;  // Marca a função como executada

}

   if (statetela == "donate" && !functionExecuted) {
        drawButtons();
        functionExecuted = true;  // Marca a função como nao executada

        }
        

  if (statetela == "clock") {

       updateClock();  

  if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }

  
if (statetela == "clock" && !functionExecuted) {
    configureClock();
  functionExecuted = true;  // Marca a função como executada

}

}

    
  


 if (statetela == "vert" && !functionExecuted) {
  
  tft.fillRectVGradient(0, 0, 480, 320, TFT_ORANGE, TFT_RED);
  tft.setCursor(0,0);
  tft.setTextSize(4);
  tft.setTextWrap(true, true); // Wrap on width and height switched off
  tft.print(mensagem.c_str());
  functionExecuted = true;  // Marca a função como executada
  
 } 
 
 
 if (statetela == "horiz" && !functionExecuted) {

  tft.fillRectHGradient(0, 0, 480, 320, TFT_MAGENTA, TFT_BLUE);
  tft.setCursor(10,10);
  tft.setTextSize(4);
  tft.print(mensagem.c_str());
  functionExecuted = true;  // Marca a função como executada

 }


 if (statetela == "scrolling") {
        Serial.println("modo scrolling");


 if (statetela == "scrolling" && !functionExecuted) {

  // Carrega a fonte personalizada

  // Configurações do texto
  tft.setTextColor(TFT_YELLOW); // Define a cor do texto
  tft.setRotation(rotate); // Define a orientação do display (1 ou 3 para rotacionar 90 graus)
  tft.setTextWrap(false, false); // Wrap on width and height switched off

  tft.setTextSize(10);
  // Limpa a tela
 // tft.fillScreen(TFT_BLACK);
  tft.fillScreen(random(0xFFFF));


  // Define a posição vertical do texto
  textY = (tft.height() - tft.fontHeight()) / 2;
    functionExecuted = true;  // Marca a função como executada

 }


    updateTextPosition();

 }
  
  
  
  
  if (statetela == "hora" && !functionExecuted) {

 if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
      hora();
      functionExecuted = true;  // Marca a função como executada

  }

   if (statetela == "horax" && !functionExecuted) {

     if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
      horax();
      functionExecuted = true;  // Marca a função como executada

  }


     if (statetela == "site" && !functionExecuted) {

     if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
    qrcodemonitor();
      functionExecuted = true;  // Marca a função como executada

  }

   if (statetela == "doa" && !functionExecuted) {

     if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
    qrcodedoa();
      functionExecuted = true;  // Marca a função como executada

  }



     if (statetela == "news" && !functionExecuted) {

     if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
           fetchAndPrintRSSWithRetry(rss_feed_url);


  }

  

    if (statetela == "olhos") {

    unsigned long currentMillis = millis();
    
    
    if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }

  if (statetela == "olhos" && !functionExecuted) {

  // Initialise the eye(s), this will set all chip selects low for the tft.init()
  initEyes();

  #ifdef USE_DMA
  tft.initDMA();
#endif


  tft.unloadFont(); // Remove the font to recover memory used
  tft.setRotation(rotate); // Ajusta a rotação do display, se necessário
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(30, 280);  // Posição do cursor na tela
  tft.setTextColor(TFT_RED, TFT_BLACK); // Change the font colour and the background colour
  tft.println("EU SOU O ROBO GrANA TOQUE NA TELA");
  tft.setCursor(150, 300);  // Posição do cursor na tela
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); // Change the font colour and the background colour
  tft.println("The Green ANArchy Project");
    
    startTime = millis(); // For frame-rate calculation

    functionExecuted = true;  // Marca a função como executada

  }


  updateEye();


    }

    

  if (statetela == "telegram" && !functionExecuted) {

    telegram();
    functionExecuted = true;  // Marca a função como executada

  }


if (statetela == "lermsgtg") {
  if (currentMillis - lastExecutionTime >= 30000) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
    if (statetela == "lermsgtg" && !functionExecuted) {

    lermsgtg();
    functionExecuted = true;  // Marca a função como executada

  }
  }

  if (statetela == "fototft") {
    


    if (statetela == "fototft" && !functionExecuted) {

    fototft();
    functionExecuted = true;  // Marca a função como executada

  }
  }

  

  
if (statetela == "chat") {
  if (currentMillis - lastExecutionTime >= 10000) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
    if (statetela == "chat" && !functionExecuted) {

    lermsgchat();
    functionExecuted = true;  // Marca a função como executada

  }
  }


 
if (statetela == "bolsa") {
  if (currentMillis - lastExecutionTime >= 60000) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
    if (statetela == "bolsa" && !functionExecuted) {

    lerbolsa();
    functionExecuted = true;  // Marca a função como executada

  }
  }
  
    if (statetela == "crypto") {

  if (currentMillis - lastExecutionTime >= 120000) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
    if (statetela == "crypto"  && !functionExecuted) {

    bolsa();
    lastExecutionTime = currentMillis;
    functionExecuted = true;  // Marca a função como executada

  }}


  

    if (statetela == "botaorelay") {

  
    if (statetela == "botaorelay"  && !functionExecuted) {
    drawButtonsrelay();
    delay(300);

    functionExecuted = true;  // Marca a função como executada

  }}
  

  

  


  

   if (statetela == "OTA"  && !functionExecuted) {

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0); // Set cursor at top left of screen

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("FIRMWARE");
  tft.setTextColor(TFT_RED, TFT_BLACK); // Change the font colour and the background colour
  tft.println("VERSAO");
  tft.println(GRANAVERSION);
  functionExecuted = true;  // Marca a função como executada

  }
  


#if (SENSORES == 1)



if (statetela == "ph") {
  if (currentMillis - lastExecutionTime >= 500) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }}

  if (statetela == "ph" && !functionExecuted) {
 
    phtela();
    functionExecuted = true;  // Marca a função como executada
  }



if (statetela == "clima") {
  if (currentMillis - lastExecutionTime >= intervalclima) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }
  
  
   if (statetela == "clima" && !functionExecuted) {
  clima();
  functionExecuted = true;  // Marca a função como executada
  }
  
  
  }

 

  #endif




  
  }

  








void hora(){
//   tft.loadFont(Font72x53rle); // Load another different font
  //tft.unloadFont(); // Remove the font to recover memory used
   // tft.setTextColor(TFT_CYAN, TFT_BLACK);
 // tft.fillRectHGradient(0, 0, 480, 320, TFT_MAGENTA, TFT_BLUE);
   //tft.fillScreen(TFT_BLACK);

   tft.setRotation(rotate);  // portrait
   tft.setCursor(0, 0); // Set cursor at top left of screen
   tft.fillScreen(random(0xFFFF));
   tft.setTextSize(4);
   tft.setTextColor(TFT_WHITE);  // Define a cor do texto como branco
    
   unsigned long currentTime = millis(); // Get current time

    // Check if it's time to update the display
    if (currentTime - lastUpdateTime >= updateInterval) {
        lastUpdateTime = currentTime; // Record the current time as the last update time
        obterHoraEDataAtual(); // Update current time
        displayTime(); // Display current time
    }
  
  }

  
void horax(){
  //  tft.loadFont(AA_FONT_LARGE); // Load another different font

   tft.setCursor(0, 0); // Set cursor at top left of screen
//  tft.fillRectHGradient(0, 0, 480, 320, TFT_MAGENTA, TFT_BLUE);
//  tft.fillRectVGradient(0, 0, 480, 320, TFT_CYAN, TFT_YELLOW);
  tft.fillRectVGradient(0, 0, 480, 320, TFT_ORANGE, TFT_GREEN);
  //tft.setTextColor(TFT_RED);  // Define a cor do texto como branco
  tft.setTextColor(TFT_CYAN, TFT_BLACK);

  tft.setTextSize(4);


    
   unsigned long currentTime = millis(); // Get current time

    // Check if it's time to update the display
    if (currentTime - lastUpdateTime >= updateInterval) {
        lastUpdateTime = currentTime; // Record the current time as the last update time
        obterHoraEDataAtual(); // Update current time
        displayTime(); // Display current time
    }
  
  }




void displayTime() {
    tft.setCursor(120, 120); // Set cursor position
    tft.printf("%02d:%02d", currentHour, currentMinute); // Print time in HH:MM format
}










void configureClock() {



  
  hh = conv2d(__TIME__);
  mm = conv2d(__TIME__ + 3);
  ss = conv2d(__TIME__ + 6);

 // tft.init();
  tft.setRotation(rotate);
  tft.setTextSize(4); // Set text size
  tft.fillScreen(TFT_BLACK);
// Define o centro da tela
  uint16_t centerX = tft.width() / 2;
  uint16_t centerY = tft.height() / 2;

  // Draw clock face
  tft.fillCircle(centerX, centerY, 118, TFT_GREEN);
  tft.fillCircle(centerX, centerY, 110, TFT_BLACK);

  // Draw 12 lines
  for (int i = 0; i < 360; i += 30) {
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * 114 + centerX;
    uint16_t yy0 = sy * 114 + centerY;
    uint16_t x1 = sx * 100 + centerX;
    uint16_t yy1 = sy * 100 + centerY;

    tft.drawLine(x0, yy0, x1, yy1, TFT_GREEN);
  }

  // Draw 60 dots
  for (int i = 0; i < 360; i += 6) {
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * 102 + centerX;
    uint16_t yy0 = sy * 102 + centerY;
    // Draw minute markers
    tft.drawPixel(x0, yy0, TFT_WHITE);

    // Draw main quadrant dots
    if (i == 0 || i == 180)
      tft.fillCircle(x0, yy0, 2, TFT_WHITE);
    if (i == 90 || i == 270)
      tft.fillCircle(x0, yy0, 2, TFT_WHITE);
  }

  tft.fillCircle(centerX, centerY + 1, 3, TFT_WHITE);
  

  // Draw text at position centerX, centerY + 140 using fonts 4
  tft.drawCentreString(horaAtual, centerX, centerY + 122, 1);
 //   tft.printf("%02d:%02d", currentHour, currentMinute); // Print time in HH:MM format

  targetTime = millis() + 1000;
}
  





void updateClock() {




 if (targetTime < millis()) {
    targetTime += 1000;
    ss++; // Advance second
    if (ss == 60) {
      ss = 0;
     // configureClock();

      mm++; // Advance minute
      if (mm > 59) {


        
        mm = 0;
        hh++; // Advance hour
        if (hh > 23) {
          hh = 0;
        }
      }
    }

    float sdeg = ss * 6;                        // 0-59 -> 0-354
    float mdeg = mm * 6 + sdeg * 0.01666667;    // 0-59 -> 0-360 - includes seconds
    float hdeg = hh * 30 + mdeg * 0.0833333;    // 0-11 -> 0-360 - includes minutes and seconds
    float hx = cos((hdeg - 90) * 0.0174532925);
    float hy = sin((hdeg - 90) * 0.0174532925);
    float mx = cos((mdeg - 90) * 0.0174532925);
    float my = sin((mdeg - 90) * 0.0174532925);
    float sx = cos((sdeg - 90) * 0.0174532925);
    float sy = sin((sdeg - 90) * 0.0174532925);

    // Redraw new hand positions
    uint16_t centerX = tft.width() / 2;
    uint16_t centerY = tft.height() / 2;
    tft.drawLine(centerX, centerY, centerX + sx * 90, centerY + sy * 90, TFT_RED);
    tft.drawLine(centerX, centerY, centerX + hx * 62, centerY + hy * 62, TFT_WHITE);
    tft.drawLine(centerX, centerY, centerX + mx * 84, centerY + my * 84, TFT_WHITE);
    tft.fillCircle(centerX, centerY + 1, 3, TFT_RED);
  }
}

static uint8_t conv2d(const char *p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}







void menu(){

 tft.setRotation(rotate);

 // tft.loadFont(AA_FONT_LARGE); // Load another different font
  tft.setTextWrap(true, true); // Wrap on width and height

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0); // Set cursor at top left of screen

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("BEM VINDO,");
  tft.setTextColor(TFT_RED, TFT_BLACK); // Change the font colour and the background colour
  tft.println("EU SOU O ROBO");
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Change the font colour and the background colour
  tft.println("GrANA");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("The Green ANArchy");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println("TOQUE NA TELA");

 //   tft.unloadFont(); // Remove the font to recover memory used
}




void telegram(){

 tft.setRotation(rotate);

 // tft.loadFont(AA_FONT_LARGE); // Load another different font
  tft.setTextWrap(true, true); // Wrap on width and height

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 0); // Set cursor at top left of screen

  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("ENVIANDO,");
  tft.setTextColor(TFT_RED, TFT_BLACK); // Change the font colour and the background colour
  tft.println("MENSAGEM");
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Change the font colour and the background colour
  tft.println("TELEGRAM");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("The Green ANArchy");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println("TOQUE NA TELA");
  delay(2000);
  telegrammsg();


 //   tft.unloadFont(); // Remove the font to recover memory used
}


//clima tela

void clima(){

tft.setRotation(rotate);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(0, 0); // Set cursor at top left of screen


    if (aht.begin()) {
      Serial.println("AHT inicializado com sucesso!");
    sensors_event_t temp;
    aht_temp->getEvent(&temp);  
    float t = temp.temperature;

    sensors_event_t humidity;
    aht_humidity->getEvent(&humidity);
    float h = humidity.relative_humidity;

    sensors_event_t pressure_event;
    bmp_pressure->getEvent(&pressure_event);
    float p = pressure_event.pressure;



    // Draw temperature bar graph
    drawBarGraph(t, "Temperatura", 0, 50, TFT_RED);

    // Draw humidity pie chart
    drawBarGraph(h, "Umidade", 0, 150, TFT_BLUE);

    // Draw pressure pie chart
    drawBarGraph(p*100, "Pressao", 0, 250, TFT_YELLOW);
    


    }else {
      Serial.println("Falha no sensor AHT!");
      tft.fillScreen(TFT_RED);
      tft.setTextColor(TFT_CYAN, TFT_RED);

      tft.setTextSize(4);
      tft.setCursor(0, 0); // Set cursor at top left of screen
      tft.println("FALHA NO SENSOR");

    }
}

//phtela
void phtela(){

    tft.setRotation(rotate);
    tft.setTextWrap(true, true); // Wrap on width and height
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(4);
    tft.setCursor(0, 0); // Set cursor at top left of screen
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.println("pH Monitor");
    tft.print(sensorValue);

    drawBarGraph(ph(), "pH", 0, 250, TFT_GREEN);
    Serial.println(ph());

/*

  sensors_event_t temp;
  aht_temp->getEvent(&temp);
  float t = temp.temperature;
  
  sensors_event_t humidity;
  aht_humidity->getEvent(&humidity);
  float h = humidity.relative_humidity;


  float vpd = calcularVPD(t, h);


  String fase = determinarFaseDeCrescimento(vpd);

  
  exibirInformacoesNaTela(vpd, fase);

*/

}





float calcularVPD(float t, float h) {
  // Cálculo do VPD
  float es = 6.112 * exp((17.67 * t) / (t + 243.5));
  float e = (h / 100.0) * es;
  float vpd = es - e;
  return vpd;
}



String determinarFaseDeCrescimento(float vpd) {
  // Determinar a fase de crescimento baseada no VPD
  if (vpd < 0.4) {
    return "Fase de germinacao";
  } else if (vpd >= 0.4 && vpd < 0.8) {
    return "Fase inicial de crescimento";
  } else if (vpd >= 0.8 && vpd < 1.2) {
    return "Fase vegetativa";
  } else if (vpd >= 1.2 && vpd < 1.6) {
    return "Fase de floracao inicial";
  } else {
    return "VPD muito alto";
  }
}


void exibirInformacoesNaTela(float vpd, String fase) {
  // Limpar a tela

  // Exibir as informações
  tft.setCursor(10, 50);
  tft.setTextSize(2);
  tft.print("VPD: ");
  tft.println(vpd);

  tft.setCursor(10, 80);
  tft.println(fase);

  // Indicar o que precisa ser melhorado
  if (vpd < 0.8) {
    tft.setCursor(10, 150);
    tft.print("Aumente a umidade!");
  } else if (vpd > 1.2) {
    tft.setCursor(10, 150);
    tft.print("Diminua a umidade!");
  }

  sensors_event_t temp;
  aht_temp->getEvent(&temp);
  float t = temp.temperature;
  
  sensors_event_t humidity;
  aht_humidity->getEvent(&humidity);
  float h = humidity.relative_humidity;

  
  if (t < 20 || t > 30) {
    tft.setCursor(10, 200);
    tft.print("Ajuste a temperatura!");
  }












  
}

// #########################################################################
//  Desenha o medidor analógico na tela
// #########################################################################
void drawBarGraph(float value, String label, int x, int y, uint16_t color) {
  int barWidth;
  String status;
   
  if (label == "Pressao") {
    if (value < 101600) {
    barWidth = map(value, 99000, 102000, 0, tft.width() - 50);
      status = "Sol";
    } else {
    barWidth = map(value, 99000, 102000, 0, tft.width() - 50);
      status = "Chuva";
    }
  } else if (label == "pH") {
    barWidth = map(value, 1, 14, 0, tft.width() - 50);

    
   }else {
    barWidth = map(value, 0, 100, 0, tft.width() - 50);
    status = ""; // Não é necessário exibir status para outras medidas
  }
  
  tft.fillRect(0, y - 20, tft.width() - 50, 20, TFT_BLACK);
  tft.fillRect(0, y - 20, barWidth, 20, color);
  tft.setCursor(x, y);
  tft.setTextColor(color);
  
  // Adicionando condições para exibir a unidade correta
  String unit;
  if (label == "Pressao") {
    unit = " hPa";
  } else if (label == "Umidade") {
    unit = " %";
  } else if (label == "Temperatura") {
    unit = " C";
  } else if (label == "pH") {
    unit = "";
  } else {
    unit = ""; // Adicione outras unidades conforme necessário
  }
  
  tft.print(label + ": " + String(value) + unit);
  tft.setCursor(x, y + 25); // Exibir status abaixo da medida
  tft.print(status);
}















void qrcodedoa() {




  Serial.println("QR CODE:  " + qrdoa);
  Serial.println("Gerando QR Code...");



  

  uint8_t* qrcode = (uint8_t*)malloc(qrcodegen_BUFFER_LEN_MAX);
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

  bool ok = qrcodegen_encodeText(qrdoa.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

  if (ok) {


  tft.unloadFont(); // Remove the font to recover memory used
  tft.setRotation(rotate); // Ajusta a rotação do display, se necessário
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(350, 0); // Set cursor at top left of screen
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("Escaneie o QrCode ");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(350, 20); // Set cursor at top left of screen
  tft.println("Apoie o projeto ");
  tft.setCursor(350, 40); // Set cursor at top left of screen
  tft.println("com qualquer valor");


     int size = qrcodegen_getSize(qrcode);
  char border = 2;
  uint16_t color;
 int xOffset = 0;
  int yOffset = 0;
  char module_size = 4;


  for (int y = -border; y < size + border; y++) {
    for (int x = -border; x < size + border; x++) {
      color = qrcodegen_getModule(qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      int pixelX = (x + border) * module_size + xOffset;
      int pixelY = (y + border) * module_size + yOffset;

      for (int i = 0; i < module_size; i++) {
        for (int j = 0; j < module_size; j++) {
          tft.drawPixel(pixelX + j, pixelY + i, color);
        }
      }
    }
  }

  
  }

  free(qrcode); // Liberar a memória alocada dinamicamente
  Serial.println("QR Code gerado e Impresso");
}





void qrcodemonitor() {

  Serial.println("QR CODE: " + site);
  Serial.println("Gerando QR Code...");



  

  uint8_t* qrcode = (uint8_t*)malloc(qrcodegen_BUFFER_LEN_MAX);
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

  bool ok = qrcodegen_encodeText(site.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

  if (ok) {
   
    print_qrcodemonitor(qrcode);
  }

  free(qrcode); // Liberar a memória alocada dinamicamente
  Serial.println("QR Code gerado e Impresso");
}


void print_qrcodemonitor(const uint8_t qrcode[]) {
  tft.setRotation(rotate);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(0, 0); // Set cursor at top left of screen
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("Escaneie o QrCode ");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Controle de qualquer");
  tft.println("lugar o ROBO");
  tft.setCursor(150, 150); // Set cursor at top left of screen

  
  int size = qrcodegen_getSize(qrcode);
  char border = 2;
  char module_size = 5;
  uint16_t color;
  int xOffset = 12;
  int yOffset = 120;

  for (int y = -border; y < size + border; y++) {
    for (int x = -border; x < size + border; x++) {
      color = qrcodegen_getModule(qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      int pixelX = (x + border) * module_size + xOffset;
      int pixelY = (y + border) * module_size + yOffset;

      for (int i = 0; i < module_size; i++) {
        for (int j = 0; j < module_size; j++) {
          tft.drawPixel(pixelX + j, pixelY + i, color);
        }
      }
    }
  }
}






void fetchAndPrintRSSWithRetry(String rss_feed_url) {
  int retryAttempts = 0;
  while (retryAttempts < 5) {
    if (fetchAndPrintRSS(rss_feed_url)) {
      return; // Se obtiver sucesso, saia da função
    }
    retryAttempts++;
    Serial.println("Tentando novamente...");
    delay(2000); // Espere antes de tentar novamente
  }
  Serial.println("Excedido o número máximo de tentativas.");
}

bool fetchAndPrintRSS(String rss_feed_url) {
  HTTPClient http;

  Serial.println("Obtendo feeds RSS...");
  Serial.println(rss_feed_url);

  http.begin(rss_feed_url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();


      // Função para substituir caracteres fora do padrão por espaços em branco
      payload.replace('\r', ' '); // Substitui carriage return por espaço em branco
      payload.replace('\n', ' '); // Substitui nova linha por espaço em branco
      payload.replace('õ', 'o'); // Substitui tabulação por espaço em branco
      payload.replace('ã', 'a'); // Substitui til por espaço em branco
      payload.replace('ç', 'c'); // Substitui cedilha por 'c'
      payload.replace('í', 'i'); // Substitui cedilha por 'c'
      payload.replace('ó', 'o'); // Substitui cedilha por 'c'
      payload.replace('á', 'a'); // Substitui cedilha por 'c'
      payload.replace('ú', 'u'); // Substitui cedilha por 'c'
      payload.replace('é', 'e'); // Substitui cedilha por 'c'
      payload.replace('ê', 'e'); // Substitui cedilha por 'c'
      payload.replace('â', 'a'); // Substitui cedilha por 'c'
      payload.replace('à', 'a'); // Substitui cedilha por 'c'
      payload.replace('ô', 'o'); 
      payload.replace('Ã', 'A'); 
      
      payload.replace("&amp;", "&");
      payload.replace("&quot;", "\"");
      payload.replace("&lt;", "<");
      payload.replace("&gt;", ">");
      payload.replace("&apos;", "'");    
      // Remove <![CDATA[ e ]]>
      payload.replace("<![CDATA[", "");
      payload.replace("]]>", "");

      // Remove tags <p> e <a> da descrição
      payload.replace("<p>", "");
      payload.replace("</p>", "");
      payload.replace("<a>", "");
      payload.replace("</a>", "");
      payload.replace("</em>", "");
      payload.replace("<em>", "");
       
      // Remove tags <strong> e </strong> sem remover o conteúdo
      payload.replace("<strong>", "");
      payload.replace("</strong>", "");
      payload.replace("<span>", "");
      payload.replace("</span>", "");
      // Remove a tag <p dir="ltr"><span>
      payload.replace("<p dir=\"ltr\">", "");

      int startPos = 0;
      int endPos = 0;
      int itemCount = 0;
      String cleanedRSS = "";

      unsigned long previousMillis = 0;
      const long interval = 15000; // intervalo de 15 segundos
      
      while (true) {

             if (tft.getTouchRawZ() > 500) {
            Serial.println("Tela tocada. Saindo do loop.stopFetching");
       
            touchCount++; 
            break;
          }

    if(statetela != "news") {
                        break;

            }


          
        unsigned long currentMillis = millis();
        
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;

          startPos = payload.indexOf("<item>", endPos); // Encontra o início de um item
          endPos = payload.indexOf("</item>", startPos); // Encontra o fim do item

          if (itemCount >= 6) { // Se não houver mais itens ou 2 itens foram exibidos
            startPos = 0; // Reinicia a exibição do primeiro item
            endPos = 0;
            itemCount = 0; // Reinicia o contador de itens exibidos
            startPos = payload.indexOf("<item>", endPos); // Encontra o início de um item
            endPos = payload.indexOf("</item>", startPos); // Encontra o fim do item
          }

          // Encontra o fim do item
          int endPos = payload.indexOf("</item>", startPos);
          if (endPos == -1) break;

          // Extrai os dados do item
          String itemData = payload.substring(startPos, endPos);

          // Extrai o título do item
          int titleStart = itemData.indexOf("<title>") + 7;
          int titleEnd = itemData.indexOf("</title>", titleStart);
          String title = itemData.substring(titleStart, titleEnd);

          // Limita o título a 15 palavras
          int spaceCount = 0;
          for (int i = 0; i < title.length(); i++) {
            if (title[i] == ' ') spaceCount++;
            if (spaceCount == 15) {
              title = title.substring(0, i);
              break;
            }
          }

          // Extrai a descrição do item
          int descStart = itemData.indexOf("<description>") + 13;
          int descEnd = itemData.indexOf("</description>", descStart);
          String description = itemData.substring(descStart, descEnd);

          // Remove URLs da descrição
          int urlStart = description.indexOf("<a href=");
          while (urlStart != -1) {
              int urlEnd = description.indexOf(">", urlStart);
              description.remove(urlStart, urlEnd - urlStart + 1);
              urlStart = description.indexOf("<a href=");
          }

          // Limita a descrição a 20 palavras
          spaceCount = 0;
          for (int i = 0; i < description.length(); i++) {
            if (description[i] == ' ') spaceCount++;
            if (spaceCount == 85) {
              description = description.substring(0, i);
              break;
            }
          }

          // Adiciona o título e a descrição limpos à string final
          cleanedRSS += "Título: " + title + "\n";
          cleanedRSS += "Descrição: " + description + "\n\n";

          // Procura o próximo item
          startPos = payload.indexOf("<item>", endPos);

          Serial.println(cleanedRSS);

          // Imprime o título e a descrição na tela
          tft.setRotation(rotate);
          tft.fillScreen(TFT_BLACK); // Limpa a tela antes de exibir as notícias
          tft.setTextColor(TFT_YELLOW);
          tft.setTextSize(2);
          tft.setCursor(10, 20);
          tft.println(title);
   
          tft.setTextColor(TFT_WHITE);
          tft.setTextSize(2);
          tft.setCursor(10, 70);
          tft.println(description);

          itemCount++;

     
        }
      }
      http.end();
      return true; // Indica que obteve com sucesso os feeds RSS
    } else {
      Serial.println("Falha ao obter o feed RSS.");
    }
  } else {
    Serial.println("Falha ao conectar-se ao servidor RSS.");
  }
  http.end();
  return false; // Indica que falhou ao obter os feeds RSS
}





void updateTextPosition() {

  
      tft.drawString(mensagem, textX, textY);
      textX -= scrollSpeed;
      mensagem.trim();
      tft.fillRect(0, textY - 9, tft.width(), tft.fontHeight() + 10, TFT_RED);
      tft.drawString(mensagem, textX, textY);


      // Verifica se o texto saiu completamente da tela
      if (textX + tft.textWidth(mensagem.c_str()) < 0) {
        // Se sim, reinicia a posição do texto para fora da tela
        textX = tft.width();
      }
    }
  




void lermsgchat() {

  // Fazer solicitação ao servidor PHP para obter mensagens
  HTTPClient http;
  http.begin(urlchat);
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



void lerbolsa() {

  // Fazer solicitação ao servidor PHP para obter mensagens
  HTTPClient http;
  http.begin(urlbolsa);
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


void displayMessage(String message) {
  tft.setTextWrap(true, false); // Wrap on width and height switched off
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.println(message);
}



#include <TJpg_Decoder.h>



// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}


void configfototft(){
    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The byte order can be swapped (set true for TFT_eSPI)
  TJpgDec.setSwapBytes(true);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
  
  }


void fototftshow(){
      String macAddress = WiFi.macAddress();

  String urlWithParams = String(urlfototft) + "?mac=" + macAddress;


    // Time recorded for test purposes
  uint32_t t = millis();

  // Fetch the jpg file from the specified URL, examples only, from imgur
  bool loaded_ok = getFile(urlWithParams, "/imagem.jpg"); // Note name preceded with "/"

  t = millis() - t;
  if (loaded_ok) { Serial.print(t); Serial.println(" ms to download"); }

  // List files stored in SPIFFS, should have the file now
 // listSPIFFS();

  t = millis();

  // Now draw the SPIFFS file
  tft.setRotation(rotate);
  TJpgDec.drawFsJpg(0, 0, "/imagem.jpg");

  t = millis() - t;
  Serial.print(t); Serial.println(" ms to draw to TFT");

  // Wait forever
 // while(1) yield();

}



// Fetch a file from the URL given and save it in SPIFFS
// Return 1 if a web fetch was needed or 0 if file already exists
bool getFile(String url, String filename) {

   //If it exists then no need to fetch it
  if (SPIFFS.exists(filename) == true) {
    Serial.println("Found " + filename);
    return 0;
  }

  Serial.println("Downloading "  + filename + " from " + url);

  // Check WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    Serial.print("[HTTP] begin...\n");


    HTTPClient http;
    // Configure server and url
    http.begin(url);

    Serial.print("[HTTP] GET...\n");
    // Start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      fs::File f = SPIFFS.open(filename, "w+");
      if (!f) {
        Serial.println("file open failed");
        return 0;
      }
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // File found at server
      if (httpCode == HTTP_CODE_OK) {

        // Get length of document (is -1 when Server sends no Content-Length header)
        int total = http.getSize();
        int len = total;

        // Create buffer for read
        uint8_t buff[128] = { 0 };

        // Get tcp stream
        WiFiClient * stream = http.getStreamPtr();

        // Read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // Get available data size
          size_t size = stream->available();

          if (size) {
            // Read up to 128 bytes
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // Write it to file
            f.write(buff, c);

            // Calculate remaining bytes
            if (len > 0) {
              len -= c;
            }
          }
          yield();
        }
        Serial.println();
        Serial.print("[HTTP] connection closed or file end.\n");
      }
      f.close();
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  return 1; // File was fetched from web
}


void fototft() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Construir a URL com o MAC como parâmetro GET
    String macAddress = WiFi.macAddress();
    String urlWithParams = String(urlfototft) + "?mac=" + macAddress;

    // Iniciar a conexão HTTP
    http.begin(urlWithParams);

    // Enviar a requisição GET
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.printf("Código de resposta HTTP: %d\n", httpResponseCode);

      if (httpResponseCode == HTTP_CODE_OK) {
        // Obter a resposta como uma string
        String response = http.getString();
        
        // Salvar a resposta como um arquivo
        File file = SPIFFS.open("/imagem.jpg", FILE_WRITE); // Alteração da extensão para .jpg
        if (!file) {
          Serial.println("Falha ao abrir o arquivo para escrever");
          return;
        }
        if (file.print(response) != response.length()) {
          Serial.println("Erro ao escrever a resposta no arquivo");
          file.close();
          return;
        }

        // Obter o tamanho do arquivo antes de fechá-lo
        size_t fileSize = file.size();
        Serial.print("Tamanho do arquivo SPIFFS: ");
        Serial.print(fileSize);
        Serial.println(" bytes");

        file.close();

        Serial.println("Imagem salva com sucesso no SPIFFS.");

        //exibirImagemSPIFFS("/imagem.jpg"); // Alteração da extensão para .jpg      


        tft.setRotation(rotate);  // portrait
        tft.fillScreen(random(0xFFFF));
        fototftshow();
     //   drawJpeg("/imagem.jpg", 0 , 0);     // 240 x 320 image
      } else {
        Serial.printf("Erro na resposta HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
      }
    } else {
      Serial.println("Erro ao fazer a requisição HTTP");
    }

    http.end();
  } else {
    Serial.println("WiFi não conectado.");
  }
}


  

#endif
