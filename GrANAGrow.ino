//###CONFIG INCIAIS ATIVACAO DE MODULOS

#define SENSORES 1
#define CAMERA 0


//#Bibliotecas REDE
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ESP32Ping.h>

// Crie um objeto client
WiFiClientSecure client;

//#Biblioteca WEBSERVER
#include <WebServer.h>

// Cria um objeto WebServer
WebServer server(80);

//#Biblioteca SENSORES
#if (SENSORES == 1)

#include <Adafruit_BMP280.h>  
#include <Adafruit_AHTX0.h> 
#include <Arduino.h>
#include "Wire.h"

Adafruit_BMP280 bmp;  // Objeto do sensor BMP280
Adafruit_AHTX0 aht;   // Objeto do sensor AHTX0
#define BMP280_ADDRESS 0x77
#define AHT_ADDRESS 0x38

Adafruit_Sensor *aht_humidity, *aht_temp;
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

#endif

#define I2C_MASTER_SCL1 18
#define I2C_MASTER_SDA1 17

//#define I2C_MASTER_SCL1 22
//#define I2C_MASTER_SDA1 21

//#define I2C_MASTER_SCL1 5 // Exemplo de pino para a segunda porta I2C
//#define I2C_MASTER_SDA1 4 // Exemplo de pino para a segunda porta I2C



// Configurao do certificado CA (Certificado de Autoridade) para confiar no servidor remoto
const char* caCert= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
     "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
     "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
     "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
     "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
     "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
     "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
     "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
     "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
     "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
     "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
     "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
     "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
     "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
     "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
     "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
     "-----END CERTIFICATE-----\n";

//#Bibliotecas 

//#Biblioteca SISTEMAS

#include "time.h"
#include "sntp.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <stdbool.h>
#include <stdint.h>






//##################### Configura IP

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns



//###################################CONFIGURA VARIAVIS


//###########################CONFIGURACAO DAS PINAGENS

      int Button1 = 0;  // Pino do bot         o
      int Button2 = 0;  // Pino do bot         o
      int obj1 = 0;  // Pino do bot         o
const int ledPin = 0;     // Pino do LED
const int relayPin = 0; // Pino GPIO para o rel         
const int relayPin2 = 0; // Pino GPIO para o rel         
const int relayPin3 = 0; // Pino GPIO para o rel         


const char* host = "santocyber.helioho.st";
String url =        "https://santocyber.helioho.st/granagw/gravatele.php";
String url6 =       "https://santocyber.helioho.st/granagw/verificaacao.php";
String urlcredito = "https://santocyber.helioho.st/granagw/apicredito.php";
String urlpix =     "https://santocyber.helioho.st/granagw/v2/pixapi.php";
String urlorders =  "https://santocyber.helioho.st/granagw/v2/api_orders.php";
String serverAddress =  "https://santocyber.helioho.st/granagw/log/receber_dados.php";
String urlsalvafoto = "https://santocyber.helioho.st/granagw/salvafoto.php";



bool wifiConnected = false;
String ssid;
String password ;
String nomedobot;
String geo;
String usuario;
void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo, const String &usuario);
void loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo, String &usuario);

void loadfile(String &estado, String &mensagem);


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


// Vari         veis para rastrear o estado do bot         o e os cliques
int objState = 0;       // Vari         vel para armazenar o estado do bot         o
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

unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;


const long interval = 30000; // Intervalo de 30 segundos
const long interval60 = 70000; // Intervalo de 30 segundos
const long interval120 = 120000; // Intervalo de 30 segundos

const long interval2 = 500;  // Intervalo desejado em milissegundos (200ms)
const long intervalupdate = 30000;  // Intervalo desejado em milissegundos (30s)





//##################BOTAO RESET
const int buttonPin = 0; // Defina o pino do botão
bool buttonState = false; // Estado atual do botão
bool lastButtonState = false; // Estado anterior do botão
unsigned long startTime = 0; // Variável para armazenar o tempo inicial
const unsigned long longPressDuration = 10000; // Duração do longo pressionamento em milissegundos




//########################## Setting clock just to be sure...

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

const char* time_zone = "CET-3CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

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


void setup() {
   Serial.begin(115200);
   Serial.println("Dispositivo GRANA iniciado");

#if (SENSORES == 1)

  // Inicialize o barramento I2C com os pinos SDA e SCL 
 //   Wire.begin(); // Inicialize a primeira porta I2C  
    Wire.begin(I2C_MASTER_SDA1, I2C_MASTER_SCL1); // Inicialize a primeira porta I2C
//  Wire.setClock(100000); 

   if (aht.begin()) {
    
   Serial.println("AHT inicializado com sucesso!");

     Serial.println("AHT10/AHT20 Found!");
  aht_temp = aht.getTemperatureSensor();
  aht_temp->printSensorDetails();

  aht_humidity = aht.getHumiditySensor();
  aht_humidity->printSensorDetails();

  }else{
    Serial.println("Falha ao inicializar o sensor AHT!");

}

   if (bmp.begin()) {
    
   Serial.println("BMP inicializado!");

     Serial.println("BMP Found!");
/* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


  }else{
    Serial.println("Falha ao inicializar o sensor BMP!");

}

#endif
  
  // Configurar o pino do 
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(Button1, INPUT); // Define o pino do bot         o como entrada
  pinMode(Button2, INPUT); // Define o pino do bot         o como entrada
  pinMode(obj1, INPUT); // Define o pino do bot         o como entrada
  pinMode(ledPin, OUTPUT);   // Define o pino do LED como sa         da
  pinMode(relayPin, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  digitalWrite(obj1, LOW); // Inicialmente, o rel          est          desligado
  digitalWrite(relayPin, LOW); // Inicialmente, o rel          est          desligado
  digitalWrite(relayPin2, LOW); // Inicialmente, o rel          est          desligado
  digitalWrite(relayPin3, LOW); // Inicialmente, o rel          est          desligado

if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");



// Carrega as credenciais Wi-Fi e realiza trim nos valores
loadWifiCredentials(ssid, password, nomedobot, geo, usuario);
ssid.trim();
password.trim();
nomedobot.trim();

// Verifica se tanto o SSID quanto a senha não estão vazios
if (ssid.length() > 0 && password.length() > 0) {
    // Tenta conectar-se à rede Wi-Fi
    WiFi.begin(ssid.c_str(), password.c_str());
    checkWiFiConnection();
    Serial.println("Conectando ao WiFi...");
    Serial.println("SSID: " + ssid);
    Serial.println("Password: " + password);
    Serial.println("Nome do bot: " + nomedobot);
    Serial.println("Geolocalização: " + geo);
    Serial.println("Usuario: " + usuario);
    
    // Aguarda conexão por até 30 segundos
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
        delay(500);
    }

    // Se não conectar, iniciar o modo AP
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Falha ao reconectar ao Wi-Fi.");
        startAPMode();
    } else {
        // Conexão bem-sucedida, iniciar o servidor web
        startWebServer();
    }
} else {
    // Se as credenciais estiverem vazias, iniciar o modo AP
    startAPMode();
}



    Serial.println("Configuração do ESP finalizada");


  
  loadfile(estado, mensagem);
  
  Serial.println("VARIAVEIS ARMAZENADA NO SPIFF");
  Serial.println(estado);
  Serial.println(mensagem);


  // Serial.setDebugOutput(true);
//#####################################################wifi scan

#if (CAMERA == 1)

    // Camera init
if (!setupCamera())
  {
    Serial.println("Camera Setup Failed!");
  }else{    Serial.println("Camera INICIADA");
}
#endif



  }








void loop() {

    
    server.handleClient();

  unsigned long currentMillis = millis();
  unsigned long currentMillis2 = millis();
    unsigned long currentMillis3 = millis();


 // Verifica se já passou o intervalo definido
  if (currentMillis - previousMillis >= interval) {
    // Atualiza o tempo de referência
    previousMillis = currentMillis;


    if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado");
  // Obtém o tempo atual em milissegundos

      telemetria();
      pingando();

    // Por exemplo, você pode imprimir uma mensagem
    Serial.println("Passaram-se 30 segundos!");
  }}


  
#if (CAMERA == 1)


 // Verifica se já passou o intervalo definido
  if (currentMillis2 - previousMillis2 >= interval60) {
    // Atualiza o tempo de referência
    previousMillis2 = currentMillis2;


    if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Conectado");
  // Obtém o tempo atual em milissegundos

         capturaimagem();

    // Por exemplo, você pode imprimir uma mensagem
    Serial.println("Passaram-se 60 segundos!");
  }}
#endif

      
#if (SENSORES == 1)
 // Verifica se já passou o intervalo definido
  if (currentMillis2 - previousMillis2 >= interval60) {
    // Atualiza o tempo de referência
    previousMillis2 = currentMillis2;

  if (aht.begin()) {
    
   Serial.println("AHT inicializado com sucesso!");
     Serial.println("DADOS SENSORES ");
  Serial.println(readDHTTemperature());
  Serial.println(readDHTHumidity());
  Serial.println(readDHTPressao());


  }else{
    Serial.println("Falha ao inicializar o sensor AHT!");

}


  }



  
  #endif
 

// Verifica se já passou o intervalo definido
  if (currentMillis3 - previousMillis3 >= interval120) {
    // Atualiza o tempo de referência
    previousMillis3 = currentMillis3;


  checkWiFiConnection();

    // Por exemplo, você pode imprimir uma mensagem
    Serial.println("Passaram-se 120 segundos!");
  }

//  Serial.println("VARIAVEIS ARMAZENADA NO SPIFF SALVAS SE DESLIGAR");
//  Serial.println("ESTADO");
 // Serial.println(estado);
 // Serial.println("MENSAGEM");
 // Serial.println(mensagem);

botaoreset();
}


 
