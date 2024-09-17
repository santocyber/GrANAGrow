//############SantoCyber###############
//###                                 #
//### SISTEMA DE CONTROLE DE          #
//### MONITORAMENTO DE PH GrANAGrow   #
//###                                 #
//### BY HUGO AB                      #
//### SANTOCYBER                      #
//### http://GITHUB.COM/SANTOCYBER    #
//###                                 #
//#####################################
//BIBLIOTECAS

#include <WiFi.h>
#include <FFat.h>
#include <WebServer.h>
#include <ArduinoJson.h>



#include "User_Setup.h"
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();


//#####################################
//WIFI
WebServer server(80);
char ssid[32];
char password[64];
bool apActive = false;

//TELA
int rotate = 3;
uint16_t x, y;
uint8_t z;


unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
const long interval = 1500; // Intervalo de 1500 milissegundos
const long interval2 = 15000; // Intervalo de 1500 milissegundos



//#####################################

void setup() {
  Serial.begin(115200);
  startFATFS();




  // Iniciar o display
  tft.init();
  tft.setRotation(rotate);
  tft.fillScreen(TFT_BLACK);
  tft.setTextWrap(true);

  // Mostrar logo na inicialização
  showLogo();





  if (loadWiFiCredentials()) {
    connectToWiFi();
    if (WiFi.status() != WL_CONNECTED) {
      startAccessPoint();
    }
  } else {
    startAccessPoint();
  }




setupPH();

  

  
}

void loop() {
  
  if (apActive) {
    server.handleClient();
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    loopPH();
  }

    if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    loopPH();
    sendDataToServer();
  }

  
    
}
