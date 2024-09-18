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
#include <Wire.h>
#include <Adafruit_BMP280.h>  // Biblioteca BMP280
#include <Adafruit_AHTX0.h>   // Biblioteca AHT10
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
String ssid, password, username,botname;
bool conectadoweb = false;


//TELA
int rotate = 3;
uint16_t x, y;
uint8_t z;


unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
const long interval = 2500; // Intervalo de 1500 milissegundos
const long interval2 = 60000; // Intervalo de 1500 milissegundos



//#####################################

void setup() {
  Serial.begin(115200);

  // Mostrar logo na inicialização
 // showLogo();
setupPH();

setupWEB();
}

void loop() {
    loopWEB();

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
