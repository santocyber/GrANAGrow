#if (TELA == 1)


//#define TFT_WIDTH 240
//#define TFT_HEIGHT 320
#define SCROLL_SPEED 150 // Altere para ajustar a velocidade de rolagem
#define SCROLL_DELAY 10 // Altere para ajustar o intervalo de rolagem





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

  if (statetela == "texto" && !functionExecuted) {
        Serial.println("modo texto");

   tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
                                          // so the anti-aliasing works

  tft.setCursor(0, 0); // Set cursor at top left of screen

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Large font
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

 // tft.loadFont(AA_FONT_LARGE); // Load another different font

  //tft.fillScreen(TFT_BLACK);
  
  // Draw changing numbers - does not work unless a filled rectangle is drawn over the old text
  for (int i = 0; i <= 99; i++)
  {
    tft.setCursor(50, 50);
    tft.setTextColor(TFT_GREEN, TFT_BLACK); // TFT_BLACK is used for anti-aliasing only
                                            // By default background fill is off
    tft.print("      "); // Overprinting old number with spaces DOES NOT WORK!
    tft.setCursor(50, 50);
    tft.print(i / 10.0, 1);

    // Adding a parameter "true" to the setTextColor() function fills character background
    // This extra parameter is only for smooth fonts!
    tft.setTextColor(TFT_GREEN, TFT_BLACK, true);
    tft.setCursor(50, 90);
    tft.print(i / 10.0, 1);
    
    delay (200);
  }

  delay(5000);

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Large font text wrapping
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour

  tft.setCursor(0, 0); // Set cursor at top left of screen

  tft.println("Large font!");

  tft.setTextWrap(true); // Wrap on width
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println("Long lines wrap to the next line");

  tft.setTextWrap(false, false); // Wrap on width and height switched off
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
  tft.println("Unless text wrap is switched off");

  tft.unloadFont(); // Remove the font to recover memory used

  delay(8000);

     functionExecuted = true;  // Marca a função como executada

  
  }


  if (statetela == "mensagem" && !functionExecuted) {
        Serial.println("modo mensagem");
  tft.setCursor(0, 0); // Set cursor at top left of screen
    tft.setTextWrap(true, true); // Wrap on width and height switched off

  // Defina a cor do texto
  tft.setTextColor(TFT_WHITE);
  // Defina o tamanho do texto
  tft.setTextSize(6);
    tft.fillScreen(TFT_BLACK); // Limpa a tela
    tft.print(mensagem); // Exibe o texto

     functionExecuted = true;  // Marca a função como executada



}




  if (statetela == "clock") {

  
  if (currentMillis - lastExecutionTime >= intervalclock) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução

  }

 configureClock();

 updateClock();  
    
  }


  if (statetela == "rainbow") {


        Serial.println("modo rainbow");




  if (targetTime < millis()) {
    targetTime = millis() + 10000;

    //rainbow_fill(); // Fill the screen with rainbow colours

    // The standard AdaFruit font still works as before
    tft.setTextColor(TFT_BLACK); // Background is not defined so it is transparent
 
    tft.setCursor (60, 5);
    tft.setTextFont(0);        // Select font 0 which is the Adafruit font
    tft.print("Original Adafruit font!");

    //tft.drawString("Original Adafruit font!",60,5,1); 

    // The new larger fonts do not need to use the .setCursor call, coords are embedded
    tft.setTextColor(TFT_BLACK); // Do not plot the background colour
    // Overlay the black text on top of the rainbow plot (the advantage of not drawing the background colour!)
    tft.drawCentreString("Font size 2", 120, 14, 2); // Draw text centre at position 120, 14 using font 2
    tft.drawCentreString("Font size 4", 120, 30, 4); // Draw text centre at position 120, 30 using font 4
    tft.drawCentreString("12.34", 120, 54, 6);       // Draw text centre at position 120, 54 using font 6

    tft.drawCentreString("12.34 is in font size 6", 120, 92, 2); // Draw text centre at position 120, 92 using font 2
    // Note the x position is the top of the font!

    // draw a floating point number
    float pi = 3.14159; // Value to print
    int precision = 3;  // Number of digits after decimal point
    int xpos = 90;      // x position
    int ypos = 110;     // y position
    int font = 2;       // font number 2
    xpos += tft.drawFloat(pi, precision, xpos, ypos, font); // Draw rounded number and return new xpos delta for next print position
    tft.drawString(" is pi", xpos, ypos, font);             // Continue printing from new x position

    tft.setTextSize(1);           // We are using a size multiplier of 1

    tft.setTextColor(TFT_BLACK);  // Set text colour to black, no background (so transparent)

    tft.setCursor(36, 150, 4);    // Set cursor to x = 36, y = 150 and use font 4
    tft.println("Transparent...");  // As we use println, the cursor moves to the next line

    tft.setCursor(30, 175);    // Set cursor to x = 30, y = 175
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text colour to white and background to black
    tft.println("White on black");

    tft.setTextFont(4);        // Select font 4 without moving cursor
    tft.setCursor(50, 210);    // Set cursor to x = 50, y = 210 without changing the font
    tft.setTextColor(TFT_WHITE);
    // By using #TFT print we can use all the formatting features like printing HEX
    tft.print(57005, HEX);    // Cursor does no move to next line
    tft.println(48879, HEX);  // print and move cursor to next line

    tft.setTextColor(TFT_GREEN, TFT_BLACK); // This time we will use green text on a black background
    tft.setTextFont(2); // Select font 2
    //Text will wrap to the next line if needed, by luck it breaks the lines at spaces!
    tft.println(" Ode to a Small Lump of Green Putty I Found in My Armpit One Midsummer Morning ");
  }
}



  if (statetela == "teste") {
  Serial.println("modo teste");
  tft.fillScreen(TFT_GREEN);
  tft.println(mensagem.c_str());

  }


 if (statetela == "vert" && !functionExecuted) {
  
  tft.fillRectVGradient(0, 0, 480, 320, TFT_ORANGE, TFT_RED);
  tft.setCursor(0,0);
  tft.setTextWrap(true, true); // Wrap on width and height switched off
  tft.print(mensagem.c_str());
  functionExecuted = true;  // Marca a função como executada
  
 } 
 
 
 
 if (statetela == "horiz" && !functionExecuted) {

  tft.fillRectHGradient(0, 0, 480, 320, TFT_MAGENTA, TFT_BLUE);
  tft.setCursor(10,10);
  tft.print(mensagem.c_str());
  functionExecuted = true;  // Marca a função como executada

 }




 if (statetela == "donate") {
 
  Serial.println("modo donate");
       
          touchEventHandlerpix();



          // Se loopCounter for 30, execute a ação
    if (loopCounter == 10) {
      isAdding = true;
      functionExecuted = false;  // Marca a função como executada
      pix();

      loopCounter = 0; // Reseta o contador de loop
      ordervalue = 5;
      touchCount = 0;  // Redefine o contador se exceder 4


      
    }
    else {
      // Aumenta o contador de loop
      loopCounter++;
     }
    

          

 if (statetela == "donate" && !functionExecuted) {

          drawButtons();


  }

  

 }


  
 if (statetela == "scrolling") {
      Serial.println("modo scrolling");
      Serial.println(mensagemstatus);

      displayScrollingText(mensagem.c_str());

  //    functionExecuted = true;  // Marca a função como executada

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
       
        if (!buttonsCreated) {
    createButtons();
    buttonsCreated = true;
  }

  }

  

    if (statetela == "olhos") {
    olhos();
  }
  

  if (statetela == "telegram" && !functionExecuted) {

    telegram();
    functionExecuted = true;  // Marca a função como executada

  }

  


#if (SENSORES == 1)



if (statetela == "ph") {
  if (currentMillis - lastExecutionTime >= intervalclima) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }}

  if (statetela == "ph" && !functionExecuted) {
       tft.setRotation(3);

    tft.fillScreen(TFT_BLACK);
   // tft.loadFont(AA_FONT_LARGE); // Load another different font


    if (aht.begin()) {
      Serial.println("AHT inicializado com sucesso!");

phtela();



    

    }else {
        
      tft.print("Falha ao inicializar o sensor AHT!");
      tft.print("sensor AHT!");
      Serial.println("Falha ao inicializar o sensor AHT!");
    }

    functionExecuted = true;  // Marca a função como executada
  }



if (statetela == "clima") {
  if (currentMillis - lastExecutionTime >= intervalclima) {
    lastExecutionTime = currentMillis;
    functionExecuted = false; // Reseta a variável para próxima execução
  }}

  if (statetela == "clima" && !functionExecuted) {
       tft.setRotation(3);

    tft.fillScreen(TFT_BLACK);
   // tft.loadFont(AA_FONT_LARGE); // Load another different font


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
        
      tft.print("Falha ao inicializar o sensor AHT!");
      tft.print("sensor AHT!");
      Serial.println("Falha ao inicializar o sensor AHT!");
    }

    functionExecuted = true;  // Marca a função como executada
  }

  #endif




  
  }

  










unsigned long lastScrollTime = 0;





void displayScrollingText(const char* text) {

      tft.setTextSize(8); // Set text size

  static unsigned long lastScrollTime = 0;
  static int16_t x = TFT_WIDTH;
  int16_t textWidth = tft.textWidth(text);
  int16_t y = (TFT_HEIGHT - tft.fontHeight()) / 2;

  // Atualiza a tela somente após o intervalo de rolagem
  if (millis() - lastScrollTime >= SCROLL_DELAY) {
    lastScrollTime = millis();
    tft.fillScreen(TFT_BLACK);
    tft.setTextWrap(false, false); // Wrap on width and height switched off

    tft.setCursor(x, y);
    tft.print(text);

    x -= SCROLL_SPEED; // Desloca o texto horizontalmente

    // Se o texto passar completamente pela tela, reinicie da direita
    if (x + textWidth < 0) {
      x = TFT_WIDTH;
    }
  }
}




void hora(){
  //  tft.loadFont(AA_FONT_LARGE); // Load another different font
  tft.unloadFont(); // Remove the font to recover memory used

   tft.setCursor(0, 0); // Set cursor at top left of screen
  tft.fillRectHGradient(0, 0, 480, 320, TFT_MAGENTA, TFT_BLUE);


 // tft.setTextColor(TFT_CYAN, TFT_BLACK);

  tft.setTextSize(12);

   //tft.fillScreen(TFT_BLACK);
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
  tft.unloadFont(); // Remove the font to recover memory used

   tft.setCursor(0, 0); // Set cursor at top left of screen
//  tft.fillRectHGradient(0, 0, 480, 320, TFT_MAGENTA, TFT_BLUE);
  tft.fillRectVGradient(0, 0, 480, 320, TFT_ORANGE, TFT_RED);


 // tft.setTextColor(TFT_CYAN, TFT_BLACK);

  tft.setTextSize(40);

   //tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);  // Define a cor do texto como branco

    
   unsigned long currentTime = millis(); // Get current time

    // Check if it's time to update the display
    if (currentTime - lastUpdateTime >= updateInterval) {
        lastUpdateTime = currentTime; // Record the current time as the last update time
        obterHoraEDataAtual(); // Update current time
        displayTime(); // Display current time
    }
  
  }




void displayTime() {
    tft.setTextSize(40); // Set text size
    tft.setCursor(120, 120); // Set cursor position
    tft.printf("%02d:%02d", currentHour, currentMinute); // Print time in HH:MM format
}










void configureClock() {


if (statetela == "clock" && !functionExecuted) {

  
  hh = conv2d(__TIME__);
  mm = conv2d(__TIME__ + 3);
  ss = conv2d(__TIME__ + 6);

 // tft.init();
  tft.setRotation(3);
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
  functionExecuted = true;  // Marca a função como executada

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

 tft.setRotation(3);

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

 tft.setRotation(3);

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




//phtela
void phtela(){
    tft.setCursor(0, 0); // Set cursor at top left of screen

    tft.fillScreen(TFT_BLACK);
   // tft.loadFont(AA_FONT_LARGE); // Load another different font
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.println("Clima Monitor");
    drawBarGraph(ph(), "pH", 0, 250, TFT_GREEN);


  sensors_event_t temp;
  aht_temp->getEvent(&temp);
  float t = temp.temperature;
  
  sensors_event_t humidity;
  aht_humidity->getEvent(&humidity);
  float h = humidity.relative_humidity;


  float vpd = calcularVPD(t, h);


  String fase = determinarFaseDeCrescimento(vpd);

  
  exibirInformacoesNaTela(vpd, fase);


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
    if (value > 93000) {
      barWidth = 0; // Barra vazia
      status = "Sol";
    } else {
      barWidth = tft.width() - 50; // Barra cheia
      status = "Chuva";
    }
  } else {
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






void touchEventHandlerpix() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) { // Changed to tft.getTouch for potential compatibility
    // Adjust mapping arguments if needed based on TFT_eSPI documentation
    x = map(x, 0, tft.width(), tft.width(), 0);
    y = map(y, 0, tft.height(), tft.height(), 0);

    checkButtons(x, y);
  }
}









void qrcodedoa() {




  Serial.println("QR CODE:  " + qrdoa);
  Serial.println("Gerando QR Code...");



  

  uint8_t* qrcode = (uint8_t*)malloc(qrcodegen_BUFFER_LEN_MAX);
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

  bool ok = qrcodegen_encodeText(qrdoa.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

  if (ok) {


  tft.unloadFont(); // Remove the font to recover memory used
  tft.setRotation(3); // Ajusta a rotação do display, se necessário
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
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
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
















const int numButtons = 10;
const int buttonWidth = 100;
const int buttonHeight = 50;
const int spacingX = 20;
const int spacingY = 20;
const int startX = 20;
const int startY = 20;
String buttonLabels[numButtons] = {"BBC", "TECMUNDO"};

void createButtons() {
  int numCols = 1; // Número de colunas
  int numRows = 2; // Número de linhas
  int buttonWidth = (tft.width() - (numCols + 1) * spacingX) / numCols;
  int buttonHeight = (tft.height() - (numRows + 1) * spacingY) / numRows;

  int currentX = spacingX;
  int currentY = spacingY;
  
  tft.unloadFont(); // Remove a fonte para recuperar a memória usada
  tft.setTextWrap(true, false); // Quebra de texto em largura e altura

  tft.setCursor(0, 0); // Define o cursor no canto superior esquerdo da tela
  tft.fillScreen(TFT_BLACK); // Limpa a tela antes de exibir as notícias

  for (int i = 0; i < numButtons; i++) {
    tft.fillRect(currentX, currentY, buttonWidth, buttonHeight, TFT_CYAN);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(6);
    tft.setCursor(currentX + 10, currentY + 15);
    tft.println(buttonLabels[i]);

    currentY += buttonHeight + spacingY;
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

      statetela = "esperando";



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
      payload.replace('ô', 'o'); // Substitui cedilha por 'c'



      int startPos = 0;
      int endPos = 0;
      int itemCount = 0;
          String cleanedRSS = "";


      while (true) {
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
        // Remove caracteres indesejados do título
        title.replace("&amp;", "&");
        title.replace("&quot;", "\"");
        title.replace("&lt;", "<");
        title.replace("&gt;", ">");
        title.replace("&apos;", "'");    
        // Remove <![CDATA[ e ]]>
        title.replace("<![CDATA[", "");
        title.replace("]]>", "");


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
        // Remove caracteres indesejados da descrição
        description.replace("&amp;", "&");
        description.replace("&quot;", "\"");
        description.replace("&lt;", "<");
        description.replace("&gt;", ">");
        description.replace("&apos;", "'");

        // Remove tags <p> e <a> da descrição
        description.replace("<p>", "");
        description.replace("</p>", "");
        description.replace("<a>", "");
        description.replace("</a>", "");
         // Remove <![CDATA[ e ]]>
        description.replace("<![CDATA[", "");
        description.replace("]]>", "");

           // Remove tags <strong> e </strong> sem remover o conteúdo
        description.replace("<strong>", "");
        description.replace("</strong>", "");
         // Remove a tag <p dir="ltr"><span>
        description.replace("<p dir=\"ltr\"><span>", "");

        

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
          if (spaceCount == 60) {
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




        /*

     
   
        String itemData = payload.substring(startPos, endPos); // Extrai os dados do item

        // Extrai o título do item
        int titleStart = itemData.indexOf("<title>") + 16;
        int titleEnd = itemData.indexOf("]]></title>", titleStart);
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
        int descStart = itemData.indexOf("<description>") + 22;
        int descEnd = itemData.indexOf("]]></description>", descStart);
        String description = itemData.substring(descStart, descEnd);
        // Limita a descrição a 20 palavras
        spaceCount = 0;
        for (int i = 0; i < description.length(); i++) {
          if (description[i] == ' ') spaceCount++;
          if (spaceCount == 60) {
            description = description.substring(0, i);
            break;
          }
        }




*/





        // Imprime o título e a descrição na tela
        tft.fillScreen(TFT_BLACK); // Limpa a tela antes de exibir as notícias
        tft.setTextColor(TFT_WHITE);
        tft.setTextSize(2);
        tft.setCursor(10, 20);
        tft.println(title);
   

        tft.setTextColor(TFT_RED);
        tft.setTextSize(3);
        tft.setCursor(10, 70);
        tft.println(description);


        delay(10000); // Atraso de 10 segundos entre cada item

        itemCount++;

           if (tft.getTouchRawZ() > 300) {
          Serial.println("Tela tocada. Saindo do loop.stopFetching");
          statetela = "menu";
          break;
        }
      
      }
      http.end();

      return true; // Indica que obteve com sucesso os feeds RSS
    } else {
      Serial.println("Falha ao obter o feed RSS.");
      createButtons();
    }
  } else {
    Serial.println("Falha ao conectar-se ao servidor RSS.");
    createButtons();
  }

  http.end();
  return false; // Indica que falhou ao obter os feeds RSS

}













#endif
