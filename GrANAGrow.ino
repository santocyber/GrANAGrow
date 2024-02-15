//#Bibliotecas REDE
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ESP32Ping.h>


#define SENSORES 1
#include <Wire.h>  // Incluindo a biblioteca do barramento I2C
#include <Adafruit_BMP280.h>  // Incluindo a biblioteca do sensor de pressão BMP280
#include <Adafruit_AHTX0.h>   // Incluindo a biblioteca do sensor de temperatura e umidade AHTX0


Adafruit_BMP280 bmp;  // Objeto do sensor BMP280
Adafruit_AHTX0 aht;   // Objeto do sensor AHTX0


// Configuração do certificado CA (Certificado de Autoridade) para confiar no servidor remoto
const char* caCert = \
 "-----BEGIN CERTIFICATE-----\n"
  "MIICtjCCAZ4CAQAwFTETMBEGA1UEAwwKdGVzLm9ibS5wdDCCASIwDQYJKoZIhvcN\n"
  "AQEBBQADggEPADCCAQoCggEBANvuYarL3NB+2S4uGc8IftHLctFT8qKAzAcJt3yJ\n"
  "iFa4dixMgxNMnNZ8t1iAMEIlm0exzQ9Jbhi0i27RGwXnUDU5sS9RESqc02ca3HSd\n"
  "DBWCc3RYxetHJTJWAIam7D7qlsHTdLBAKUKKhytHD+uFYcC0D44IHfUmXmoQva9c\n"
  "QNSWeFZJQOywcclQyaThjSpg9xbg09Nx1/JLRV4ocKMnSm1oR9EkGu1oI8ppgeQv\n"
  "wrEyPYniRlNY0uT7K+NvCKZ46J1naPrnWwleqxp/JGfykH63u6s4VLZEQ7ibmAI3\n"
  "WX3hXTcGYxcz3LvwZNjpRslAX3iFLr9docf0HjSbF70zcv0CAwEAAaBcMFoGCSqG\n"
  "SIb3DQEJDjFNMEswCQYDVR0TBAIwADALBgNVHQ8EBAMCBeAwMQYDVR0RBCowKIIK\n"
  "dGVzLm9ibS5wdIIMKi50ZXMub2JtLnB0ggwqLnRlcy5vYm0ucHQwDQYJKoZIhvcN\n"
  "AQELBQADggEBALYRG8PoLow3R6Z7X9ZntRvHYksRhaUsgmOqgqB/H2SjEFZ0kTvY\n"
  "0BIG98FFfHRG8omo/jlsPBBD/el8WzrBNRWvennpvWRteBheM5eSruHjZO5tOwHd\n"
  "b+A6i8GmMpQVAzx31oM+fNXUzUDUGtltIxxIQMsclkqiGWuT56QZ2oZEtVTOmQP9\n"
  "NPsmirCe0CfN47LgHz73X1wjp6nd09yloR/pFO4H+gruyh+NwnKp9wP4Pq/3i6g7\n"
  "hl0mVBvlvMs/7Jt3guvcdSHPc2yyEiarRUPUxCzrjG8RRr7HpY7xkKYRI06ROPsj\n"
  "LOYRaRnaPslLfcSXH1MsIjXRgq077tzeHdo=\n"
  "-----END CERTIFICATE-----\n";


//#Bibliotecas MULTITASK
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp32-hal-psram.h"
#include <esp_system.h>
#include <esp_task_wdt.h>
#include "esp32-hal.h"

RTC_DATA_ATTR bool bootTriggered = false;

//#Biblioteca SISTEMAS
#include "Arduino.h"
#include "time.h"
#include "sntp.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <stdbool.h>
#include <stdint.h>

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
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(1, 1, 1, 1); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns



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

const long interval2 = 500;  // Intervalo desejado em milissegundos (200ms)
const long intervalupdate = 30000;  // Intervalo desejado em milissegundos (30s)



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
   Serial.begin(115200);


   // Verifica se o boot foi causado pelo watchdog
  if (bootTriggered) {
    Serial.println("############Reiniciado pelo RTC watchdog!");
  }
  // Configura o RTC watchdog para reiniciar o sistema após 60 segundos
  esp_task_wdt_init(60, true);
  // Reinicia o watchdog para evitar reinício imediato
  esp_task_wdt_reset();
  

  // Inicialize o barramento I2C com os pinos SDA e SCL específicos
  Wire.begin(8, 9); // Inicializando o barramento I2C com SDA no pino 8 e SCL no pino 9
  Wire.setClock(100000); 
  bmp.begin();
  aht.begin();
  
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
  
  Serial.println("VARIAVEIS ARMAZENADA NO SPIFF");
  Serial.println(estado);
  Serial.println(mensagem);


  // Serial.setDebugOutput(true);
//#####################################################wifi scan
  setClock();  

  }








void loop() {

    // Reinicializa o watchdog a cada loop
    esp_task_wdt_reset();



    


      telemetria();
delay(30000);

  Serial.println("SENSORES TEMPERATURA");
  Serial.println(readDHTTemperature());
  Serial.println(readDHTHumidity());
  Serial.println(readDHTPressao());
  
  Serial.println("VARIAVEIS ARMAZENADA NO SPIFF");
  Serial.println("ESTADO");
  Serial.println(estado);
  Serial.println("MENSAGEM");
  Serial.println(mensagem);


}

 
