//#Bibliotecas REDE
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ESP32Ping.h>

//#Bibliotecas MULTITASK
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

//#Biblioteca SISTEMAS
#include "Arduino.h"
#include "time.h"
#include "sntp.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <stdbool.h>
#include <stdint.h>

//#Biblioteca QRCODE
#include "qrcodegen.h"

//#Biblioteca DISPLAY
#define ILI9488_DRIVER     // WARNING: Do not connect ILI9488 display SDO to MISO if other devices share the SPI bus (TFT SDO does NOT tristate when CS is high)

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define SMOOTH_FONT

#define SPI_FREQUENCY  27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

#define TS_MINX 0
#define TS_MAXX 479
#define TS_MINY 0
#define TS_MAXY 319

#define TFT_SCLK 46
#define TFT_MOSI 9
#define TFT_DC 10
#define TFT_RST -1
#define TFT_CS 14
#define TFT_BL 8  // LED back-light (only for ST7789 with backlight control pin)
#define TFT_WIDTH  320
#define TFT_HEIGHT 480
#define TOUCH_CS 18     // Chip select pin (T_CS) of touch screen
#define T_IRQ 21
#define TOUCH_DO 3
#define TOUCH_DIN 16

#include <TFT_eSPI.h>
#include "User_Setup.h"
#include <Adafruit_GFX.h>
#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(TOUCH_CS, T_IRQ); //XPT2046_Touchscreen ts(TOUCH_CS, T_IRQ, TOUCH_DIN, TOUCH_DO);  // Create TouchScreen instance


// Crie um objeto tft
TFT_eSPI tft;

// Crie um objeto client
WiFiClientSecure client;

// Cria um objeto WebServer
WebServer server(80);

//##################### Configura IP

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns


//##############CONFIGURA TASKHANDLE

TaskHandle_t xHandle = NULL;
#define STACK_SIZE 10000
 // Structure that will hold the TCB of the task being created.
 StaticTask_t xTaskBuffer;
 StackType_t xStack[ STACK_SIZE ];


//###################################CONFIGURA VARIAVIS


//###########################CONFIGURACAO DAS PINAGENS

      int Button1 = 0;  // Pino do botão
      int Button2 = 4;  // Pino do botão
      int obj1 = 42;  // Pino do botão
const int ledPin = 48;     // Pino do LED
const int relayPin = 1; // Pino GPIO para o relé
const int relayPin2 = 2; // Pino GPIO para o relé
const int relayPin3 = 3; // Pino GPIO para o relé




const char* host = "santocyber.helioho.st";
String url =        "https://santocyber.helioho.st/granagw/gravatele.php";
String url6 =       "https://santocyber.helioho.st/granagw/verificaacao.php";
String urlcredito = "https://santocyber.helioho.st/granagw/apicredito.php";
String urlpix =     "https://santocyber.helioho.st/granagw/v2/pixapi.php";
String urlorders =  "https://santocyber.helioho.st/granagw/v2/api_orders.php";


bool wifiConnected = false;
String ssid;
String password;
String nomedobot;
String geo;
void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo);
void loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo);


//##############CONFIG DOS CONTADORES
uint32_t last_id;
uint32_t contaobj;
uint32_t contagrana;
uint32_t contagrana1;
uint32_t contagrana2;
uint32_t contagrana3;



uint32_t read_nvs_data();
uint32_t contagrana_data();
uint32_t contaobj_data();

void save_nvs_data(uint32_t data);
void contagrana_data(uint32_t data);
void contaobj_data(uint32_t data);

char info[30];

char buffer[300];


String horaAtual;
String dataAtual;

String State = "menu";
String StateUpdate = "desativado";
String payload;
String payloadx;
String payloadxx;
String pagamento;
String estado;
String mensagem;

int ordervalue = 1;


// Variáveis para rastrear o estado do botão e os cliques
int buttonState = 1;       // Variável para armazenar o estado do botão
int objState = 0;       // Variável para armazenar o estado do botão
int contaobjfunc = 0;
bool functionExecuted = false;
bool functionExecutedX = false;


int contacendled = 0;
int conta = 1;
unsigned long intervalo = 0;
unsigned long intervaloX = 0;
unsigned long intervaloXX = 0;
int verificacoes = 0;


//###########################################CONFIGURA O LOOP DE UPDATE NO SERVDIOR SQL
unsigned long previousMillis = 0;
const long interval = 30000; // Intervalo de 30 segundos




//########################## Setting clock just to be sure...

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

const char* time_zone = "CET-3CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

String obterHoraAtual() {

    time_t agora = time(nullptr); // Obtemos a hora atual
    struct tm infoTempo;
    localtime_r(&agora, &infoTempo); // Converte o tempo em uma estrutura tm

    char buffer[20]; // Buffer para armazenar a hora formatada (HH:MM:SS\0)
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d_%02d/%02d/%04d", infoTempo.tm_hour, infoTempo.tm_min, infoTempo.tm_sec, infoTempo.tm_mday, infoTempo.tm_mon + 1, infoTempo.tm_year + 1900);
             
    return String(buffer);
}


void obterHoraEDataAtual() {
    time_t agora = time(nullptr); // Obtemos a hora atual
    struct tm infoTempo;
    localtime_r(&agora, &infoTempo); // Converte o tempo em uma estrutura tm

    char horaBuffer[9]; // Buffer para armazenar a hora formatada (HH:MM:SS\0)
    snprintf(horaBuffer, sizeof(horaBuffer), "%02d:%02d:%02d", infoTempo.tm_hour, infoTempo.tm_min, infoTempo.tm_sec);

    char dataBuffer[11]; // Buffer para armazenar a data formatada (DD/MM/YYYY\0)
    snprintf(dataBuffer, sizeof(dataBuffer), "%02d/%02d/%04d", infoTempo.tm_mday, infoTempo.tm_mon + 1, infoTempo.tm_year + 1900);

    horaAtual = String(horaBuffer);
    dataAtual = String(dataBuffer);
}


void setClock() {

  sntp_servermode_dhcp(1);    // (optional)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}






bool initWiFi() {
  client.setCACert(caCert);
  loadWifiCredentials(ssid, password, nomedobot,geo);
  ssid.trim();
  password.trim();
  nomedobot.trim();
  
  WiFi.mode(WIFI_STA);
  
  if(ssid==""){
    Serial.println("SSID indefindo");
    return false;
  }

 // Verifique se o SSID e a senha não estão vazios
  if (ssid.length() > 0 && password.length() > 0) {

    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");
    Serial.println(ssid.c_str());
    Serial.println(password.c_str());
    Serial.println(nomedobot.c_str());
    Serial.println(geo.c_str());
    }
   delay(5000);
  if (WiFi.status() == WL_CONNECTED) {
  Serial.println("conectado");
  WiFi.setHostname(nomedobot.c_str());
  localIP;
  localGateway;
  Serial.println(WiFi.localIP());
  StateUpdate = "ativo";



 if (!MDNS.begin(nomedobot.c_str())) {
    Serial.println("Error setting up MDNS responder!");
    return false;
  } else {
    MDNS.addService("http", "tcp", 80);
    Serial.printf("mDNS responder started '%s'\n", nomedobot.c_str());
    Serial.print("You can now connect to http://");
    Serial.print(nomedobot.c_str());
    Serial.println(".local");   

 }

    return true;

  
  if (!WiFi.config(localIP, localGateway, subnet,primaryDNS)){
    Serial.println("STA Failed to configure");
    return false;
  }
    } else {
    Serial.println("nao conectado");
   return false;

    }
  }


void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
   Serial.begin(115200);

  //pinMode(T_IRQ, INPUT_PULLUP);  // Set T_IRQ pin as input with pull-up resistor

  
  tft.init();
  tft.setTextFont(0); // Configura a fonte de texto para o índice 1
  tft.setRotation(2);

  ts.begin();
  ts.setRotation(2);
 

  tft.fillScreen(TFT_RED);  // Limpa a tela com fundo preto
  tft.setTextColor(TFT_WHITE);  // Define a cor do texto como branco
  tft.setTextSize(3);  // Define o tamanho do texto

  // Escreve "Hello, World!" no centro da tela
  tft.setCursor(50, 100);  // Posição do cursor na tela
  tft.println("BEM VINDO,");
  tft.println("ESSE EH O ROBO DE");
  tft.println("PAGAMENTO DA ");
 
  tft.setTextColor(TFT_GREEN);  // Define a cor do texto como branco
  tft.setTextSize(5);  // Define o tamanho do texto
  tft.println("GrANANET!");



  
  // Configurar o pino do relé como saída
  pinMode(Button1, INPUT); // Define o pino do botão como entrada
  pinMode(Button2, INPUT); // Define o pino do botão como entrada
  pinMode(obj1, INPUT); // Define o pino do botão como entrada
  pinMode(ledPin, OUTPUT);   // Define o pino do LED como saída
  pinMode(relayPin, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  digitalWrite(obj1, LOW); // Inicialmente, o relé está desligado
  digitalWrite(relayPin, LOW); // Inicialmente, o relé está desligado
  digitalWrite(relayPin2, LOW); // Inicialmente, o relé está desligado
  digitalWrite(relayPin3, LOW); // Inicialmente, o relé está desligado

if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");

  if(initWiFi()) {
  server.on("/", handleMain);
 // server.on("/desligar", handleDesligar);
 // server.on("/ligar", handleLigar);
  server.begin();
  Serial.println("Servidor web iniciado");
  }

else {
// Iniciar o modo AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP("GRANANET-Config"); // Nome da rede AP
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Endereço IP do AP: ");
  Serial.println(myIP);
  // Inicializa a biblioteca WiFi
  WiFi.begin();

    // Inicializa o servidor web
  server.on("/", handleRoot);
  server.on("/scan", HTTP_GET, handleScan);
  server.on("/connect", handleConnect);
 // server.on("/desligar", handleDesligar);
 // server.on("/ligar", handleLigar);
  server.begin();
  Serial.println("Servidor web AP iniciado");

}

  Serial.println("ESP Setup finalizado");
  
  loadfile(estado, mensagem);
  last_id = read_nvs_data();
  contagrana = read_contagrana();
  contaobj = read_contaobj();
  
  Serial.println("VARIAVEIS ARMAZENADA NO TXT");
  Serial.println(estado);
  Serial.println(contagrana);
  Serial.println(contaobj);
  Serial.println(last_id);

  // Serial.setDebugOutput(true);
//#####################################################wifi scan
  setClock();  




//############################################ Configura LOOP TASKHANDLE
 
 //xStack = (uint8_t*)heap_caps_calloc(1, 5000, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT | MALLOC_CAP_32BIT);
//pxStackBuffer = (uint8_t*)heap_caps_calloc(1, 5000, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT | MALLOC_CAP_32BIT);
  delay(200);


     // Create the task pinned to core 0 without using any dynamic memory allocation.
    //      xHandle = xTaskCreateStatic(
     xHandle = xTaskCreateStaticPinnedToCore(
                   TELE,       // Function that implements the task.
                   "TELE",          // Text name for the task.
                   STACK_SIZE,      // Stack size in bytes, not words.
                   ( void * ) 1,    // Parameter passed into the task.   ( void * ) 1
                   1,// Priority at which the task is created.
                   xStack,          // Array to use as the task's stack.
                   &xTaskBuffer,    // Variable to hold the task's data structure.
                   1 );     




}



 
