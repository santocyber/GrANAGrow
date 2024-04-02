//###Green Anarchy project GrANAGROW
//###by SantoCyber
//###segue no YT @RastaNerdi
//###
//###


//###CONFIG INCIAIS ATIVACAO DE MODULOS

#define SENSORES 1
#define CAMERA 0
#define TELA 1


//#Bibliotecas REDE
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ESPping.h>

//#Biblioteca WEBSERVER
#include <WebServer.h>

// Cria um objeto WebServer
WebServer server(80);

//#Biblioteca SENSORES
#if (SENSORES == 1)

#include <Adafruit_BMP280.h>  
#include <Adafruit_AHTX0.h> 
#include "Arduino.h"

Adafruit_BMP280 bmp;  // Objeto do sensor BMP280
Adafruit_AHTX0 aht;   // Objeto do sensor AHTX0
//#define BMP280_ADDRESS 0x77
//#define AHT_ADDRESS 0x38

Adafruit_Sensor *aht_humidity, *aht_temp;
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

#endif

//#define I2C_MASTER_SCL1 18
//#define I2C_MASTER_SDA1 17

#define I2C_MASTER_SCL1 25
#define I2C_MASTER_SDA1 26

//#define I2C_MASTER_SCL1 22
//#define I2C_MASTER_SDA1 21

//#define I2C_MASTER_SCL2 5 // Exemplo de pino para a segunda porta I2C
//#define I2C_MASTER_SDA2 4 // Exemplo de pino para a segunda porta I2C






//##################### Configura IP

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns
IPAddress primaryDNS(9, 9, 9, 9); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns

//heliohost dns ip 65.19.143.3
//IPAddress primaryDNS(65, 19, 143, 3); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns


#include <WiFiClientSecure.h>

WiFiClientSecure client;


// Configuração do certificado CA (Certificado de Autoridade) para confiar no servidor remoto
const char* caCert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIF6TCCA9GgAwIBAgIQBeTcO5Q4qzuFl8umoZhQ4zANBgkqhkiG9w0BAQwFADCB\n" \
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTQw\n" \
"OTEyMDAwMDAwWhcNMjQwOTExMjM1OTU5WjBfMQswCQYDVQQGEwJGUjEOMAwGA1UE\n" \
"CBMFUGFyaXMxDjAMBgNVBAcTBVBhcmlzMQ4wDAYDVQQKEwVHYW5kaTEgMB4GA1UE\n" \
"AxMXR2FuZGkgU3RhbmRhcmQgU1NMIENBIDIwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
"DwAwggEKAoIBAQCUBC2meZV0/9UAPPWu2JSxKXzAjwsLibmCg5duNyj1ohrP0pIL\n" \
"m6jTh5RzhBCf3DXLwi2SrCG5yzv8QMHBgyHwv/j2nPqcghDA0I5O5Q1MsJFckLSk\n" \
"QFEW2uSEEi0FXKEfFxkkUap66uEHG4aNAXLy59SDIzme4OFMH2sio7QQZrDtgpbX\n" \
"bmq08j+1QvzdirWrui0dOnWbMdw+naxb00ENbLAb9Tr1eeohovj0M1JLJC0epJmx\n" \
"bUi8uBL+cnB89/sCdfSN3tbawKAyGlLfOGsuRTg/PwSWAP2h9KK71RfWJ3wbWFmV\n" \
"XooS/ZyrgT5SKEhRhWvzkbKGPym1bgNi7tYFAgMBAAGjggF1MIIBcTAfBgNVHSME\n" \
"GDAWgBRTeb9aqitKz1SA4dibwJ3ysgNmyzAdBgNVHQ4EFgQUs5Cn2MmvTs1hPJ98\n" \
"rV1/Qf1pMOowDgYDVR0PAQH/BAQDAgGGMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYD\n" \
"VR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMCIGA1UdIAQbMBkwDQYLKwYBBAGy\n" \
"MQECAhowCAYGZ4EMAQIBMFAGA1UdHwRJMEcwRaBDoEGGP2h0dHA6Ly9jcmwudXNl\n" \
"cnRydXN0LmNvbS9VU0VSVHJ1c3RSU0FDZXJ0aWZpY2F0aW9uQXV0aG9yaXR5LmNy\n" \
"bDB2BggrBgEFBQcBAQRqMGgwPwYIKwYBBQUHMAKGM2h0dHA6Ly9jcnQudXNlcnRy\n" \
"dXN0LmNvbS9VU0VSVHJ1c3RSU0FBZGRUcnVzdENBLmNydDAlBggrBgEFBQcwAYYZ\n" \
"aHR0cDovL29jc3AudXNlcnRydXN0LmNvbTANBgkqhkiG9w0BAQwFAAOCAgEAWGf9\n" \
"crJq13xhlhl+2UNG0SZ9yFP6ZrBrLafTqlb3OojQO3LJUP33WbKqaPWMcwO7lWUX\n" \
"zi8c3ZgTopHJ7qFAbjyY1lzzsiI8Le4bpOHeICQW8owRc5E69vrOJAKHypPstLbI\n" \
"FhfFcvwnQPYT/pOmnVHvPCvYd1ebjGU6NSU2t7WKY28HJ5OxYI2A25bUeo8tqxyI\n" \
"yW5+1mUfr13KFj8oRtygNeX56eXVlogMT8a3d2dIhCe2H7Bo26y/d7CQuKLJHDJd\n" \
"ArolQ4FCR7vY4Y8MDEZf7kYzawMUgtN+zY+vkNaOJH1AQrRqahfGlZfh8jjNp+20\n" \
"J0CT33KpuMZmYzc4ZCIwojvxuch7yPspOqsactIGEk72gtQjbz7Dk+XYtsDe3CMW\n" \
"1hMwt6CaDixVBgBwAc/qOR2A24j3pSC4W/0xJmmPLQphgzpHphNULB7j7UTKvGof\n" \
"KA5R2d4On3XNDgOVyvnFqSot/kGkoUeuDcL5OWYzSlvhhChZbH2UF3bkRYKtcCD9\n" \
"0m9jqNf6oDP6N8v3smWe2lBvP+Sn845dWDKXcCMu5/3EFZucJ48y7RetWIExKREa\n" \
"m9T8bJUox04FB6b9HbwZ4ui3uRGKLXASUoWNjDNKD/yZkuBjcNqllEdjB+dYxzFf\n" \
"BT02Vf6Dsuimrdfp5gJ0iHRc2jTbkNJtUQoj1iM=\n" \
"-----END CERTIFICATE-----\n";




//##############CONFIG TASKHANDLE
// Define o tamanho da pilha para ambas as tarefas.
#define STACK_SIZE 5000

// Define os identificadores de tarefa.
TaskHandle_t xHandle_TELE = NULL;
TaskHandle_t xHandle_CAM = NULL;


// Define os buffers e pilhas de tarefa estáticos.
StaticTask_t xTaskBuffer_TELE, xTaskBuffer_CAM;
StackType_t xStack_TELE[STACK_SIZE], xStack_CAM[STACK_SIZE];






//#Bibliotecas 

//#Biblioteca SISTEMAS
#include <esp_task_wdt.h>
#include "time.h"
#include "esp_sntp.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <stdbool.h>
#include <stdint.h>


#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();



//###################################CONFIGURA VARIAVIS


//###########################CONFIGURACAO DAS PINAGENS

#define phpin 33

      int Button1 = 0;  // Pino do bot         o
      int Button2 = 0;  // Pino do bot         o
      int obj1 = 0;  // Pino do bot         o
const int ledPin = 0;     // Pino do LED
const int relayPin = 0; // Pino GPIO para o rel         
const int relayPin2 = 0; // Pino GPIO para o rel         
const int relayPin3 = 0; // Pino GPIO para o rel         


const char* host = "santocyber.helioho.st";
String site =        "http://santocyber.helioho.st/";
String url =        "http://santocyber.helioho.st/granagw/gravatele.php";
String urlsalvafoto = "http://santocyber.helioho.st/granagw/salvafoto.php";
String urlverifica = "http://santocyber.helioho.st/granagw/verificaacao.php";
String qrdoa =        "00020101021226900014BR.GOV.BCB.PIX2568pix-qr.mercadopago.com/instore/p/v2/dbb558faf3c14a7a98e7e8fb805f6c1643530016com.mercadolibre0129https://mpago.la/pos/162635835204000053039865802BR5922Cooperativa mirako org6009SAO PAULO62070503***6304C530";


bool wifiConnected = false;
String ssid;
String password ;
String nomedobot;
String geo;
String usuario;
void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo, const String &usuario);
void loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo, String &usuario);

void loadfile(String &estado, String &mensagem);
void loadTime(String &hrliga, String &hrdesliga, String &timerautomatico, String &timerfoto, String &timerfotostatus);





 
//##############CONFIG DOS CONTADORES
uint32_t last_id;


char info[30];

char buffer[300];


String horaAtual;
String dataAtual;

String StateUpdate = "desativado";
String payload;
String payloadx;
String payloadxx;
String pagamento;
String estado;
String mensagem;
String mensagemstatus;
String hrliga;
String hrdesliga;
String timerautomatico;
String timerfotostatus;
String timerfoto = "10";
String statetela;
String statetoque;
String msgtg;

String inputText = ""; // Variável para armazenar o texto digitado

char keys[5][11] = {
    {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '<'}, 
    {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '!'},
    {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '$', '%'},
    {'z', 'x', 'c', 'v', 'b', 'n', 'm', '.', '_', '*', '@'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '?', '&', '#'}
};
char key = keys[5][11];
const char NO_KEY = '\0'; // Definindo NO_KEY como um caractere nulo

int buttonIndex = -1;


int ordervalue = 1;
int loopCounter = 0; // contador de loop
int contador = 0;


// Vari         veis para rastrear o estado do bot         o e os cliques
int objState = 0;       // Vari         vel para armazenar o estado do bot         o
int contaobjfunc = 0;
bool functionExecuted = false;
bool Executed = true;
bool buttonsCreated = false; // Variável para controlar se os botões foram criados
bool stopFetching = false;


int contacendled = 0;
int conta = 1;
unsigned long intervalo = 0;
unsigned long intervaloX = 0;
unsigned long intervaloXX = 0;
int verificacoes = 0;


//###########################################CONFIGURA O LOOP DE UPDATE NO SERVDIOR SQL
static int touchCount = 0;

unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

const long intervalo5 = 5000; // Intervalo de 30 segundos

const long interval = 30000; // Intervalo de 30 segundos
const long interval60 = 70000; // Intervalo de 30 segundos
const long interval120 = 120000; // Intervalo de 30 segundos

const long interval2 = 500;  // Intervalo desejado em milissegundos (200ms)
const long intervalupdate = 30000;  // Intervalo desejado em milissegundos (30s)

unsigned long updateInterval = 500; // Update interval in milliseconds (1 second)
unsigned long intervalclima = 10000; // Intervalo de 10 segundos em milissegundos
unsigned long intervalclock = 60000; // Intervalo de 10 segundos em milissegundos

unsigned long lastExecutionTime = 0;





//##################BOTAO RESET
const int buttonPin = 0; // Defina o pino do bot��o
bool buttonState = false; // Estado atual do bot��o
bool lastButtonState = false; // Estado anterior do bot��o
unsigned long startTime = 0; // Vari��vel para armazenar o tempo inicial
const unsigned long longPressDuration = 10000; // Dura����o do longo pressionamento em milissegundos




//########################## Setting clock just to be sure...

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

const char* time_zone = "CET-3CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)


int currentHour = 0;
int currentMinute = 0;


unsigned long lastUpdateTime = 0;










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

    currentHour = infoTempo.tm_hour;
    currentMinute = infoTempo.tm_min;
}




void setup() {
   Serial.begin(115200);
    //   Serial.setDebugOutput(true);
       
  esp_task_wdt_init(240, true); //enable panic so ESP32 restarts
 // esp_task_wdt_add(NULL); //add current thread to WDT watch

  
   Serial.println("Dispositivo GRANA iniciado");




    
#if (SENSORES == 1)

  // Inicialize o barramento I2C com os pinos SDA e SCL 
 //   Wire.begin(); // Inicialize a primeira porta I2C  
    Wire.begin(I2C_MASTER_SDA1, I2C_MASTER_SCL1); // Inicialize a primeira porta I2C
 //   Wire.begin(I2C_MASTER_SDA2, I2C_MASTER_SCL2); // Inicialize a primeira porta I2C
 // Wire.setClock(100000); 

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

  last_id = read_nvs_data();


// Carrega as credenciais Wi-Fi e realiza trim nos valores
loadWifiCredentials(ssid, password, nomedobot, geo, usuario);
ssid.trim();
password.trim();
nomedobot.trim();

Serial.println("SSID: " + ssid);
Serial.println("Password: " + password);
Serial.println("Nome do bot: " + nomedobot);
Serial.println("Geolocalização: " + geo);
Serial.println("Usuário: " + usuario);

// Verifica se tanto o SSID quanto a senha não estão vazios
if (ssid.length() > 0 && password.length() > 0) {
    // Tenta conectar-se à rede Wi-Fi
    WiFi.begin(ssid.c_str(), password.c_str());
    // checkWiFiConnection();
    Serial.println("Conectando ao WiFi...");
    //client.setInsecure();//skip verification

    // Aguarda conexão por até 10 segundos
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
    }

    // Se não conectar, iniciar o modo AP
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Falha ao reconectar ao Wi-Fi.");
        startAPMode();
        scanWiFiNetworks();

    } else {
        Serial.println("Conexão bem-sucedida, iniciar o servidor web e o update ativo");

        // Conexão bem-sucedida, iniciar o servidor web
        StateUpdate = "ativo";
        startWebServer();
    }
} else {
    // Se as credenciais estiverem vazias, iniciar o modo AP
    startAPMode();
    scanWiFiNetworks();

}


    Serial.println("Config do ESP finalizada");

    
#if (TELA == 1)

   Serial.println("iniciando tela");
    tft.init();
    tft.setRotation(3);


     // Se não conectar, iniciar o modo AP
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Falha ao reconectar ao Wi-Fi.");
  tft.fillScreen(TFT_RED);
  tft.setTextSize(5);
  tft.setCursor(0, 0); // Set cursor at top left of screen
  tft.setCursor(10, 10);  // Posição do cursor na tela
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("Nao conectado");
  delay(2000);

        
    } else {
        Serial.println("Conexão bem-sucedida, iniciar o servidor web e o update ativo");
        configwifi();
    }
    
    menu();

#endif


  loadfile(estado, mensagem, mensagemstatus);
  loadTime(hrliga, hrdesliga, timerautomatico, timerfoto, timerfotostatus);

  
    Serial.println("VARIAVEIS ARMAZENADA NO SPIFF");
    Serial.println(estado);
    Serial.println(mensagem);
    Serial.println(mensagemstatus);

    Serial.println(hrliga);
    Serial.println(hrdesliga);
    Serial.println(timerautomatico);
    Serial.println(timerfotostatus);
    Serial.println(timerfoto);





//#####################################################wifi scan

#if (CAMERA == 1)

    // Camera init
if (!setupCamera())
  {
    Serial.println("Camera Setup Failed!");
  }else{    Serial.println("Camera INICIADA");
}
#endif




// Create the first task pinned to core 0.
xHandle_TELE = xTaskCreateStaticPinnedToCore(
    TELE,            // Function that implements the task.
    "TELE",          // Text name for the task.
    STACK_SIZE,      // Stack size in bytes, not words.
    (void *)1,       // Parameter passed into the task. (void *)1
    2,               // Priority at which the task is created.
    xStack_TELE,     // Array to use as the task's stack.
    &xTaskBuffer_TELE, // Variable to hold the task's data structure.
    1                // Core number to which the task should be pinned.
);


  
#if (CAMERA == 1)

// Create the second task pinned to core 0.
xHandle_CAM = xTaskCreateStaticPinnedToCore(
    CAM,             // Function that implements the task.
    "CAM",           // Text name for the task.
    STACK_SIZE,      // Stack size in bytes, not words.
    (void *)1,       // Parameter passed into the task. (void *)1
    3,               // Priority at which the task is created.
    xStack_CAM,      // Array to use as the task's stack.
    &xTaskBuffer_CAM, // Variable to hold the task's data structure.
    1                // Core number to which the task should be pinned.
);


#endif

  }








void loop() {

    esp_task_wdt_reset();

    server.handleClient();

  unsigned long currentMillis = millis();

 #if (TELA == 1)

   
 // Verifica se passou o intervalo definido
  if (currentMillis - previousMillis >= interval60) {
    // Atualiza o tempo de referencia
    previousMillis = currentMillis;
    

   Serial.println("VERIFICA TELA SQL!");
   touchCount = 0;
   verificasqltela();

  }


     if (currentMillis - previousMillis2 >= 300) {
    // Atualiza o tempo de referencia
    previousMillis2 = currentMillis;
    toque();

}  

if (currentMillis - previousMillis3 >= 1000) {
    // Atualiza o tempo de referencia
    previousMillis3 = currentMillis;
    tela();


}

#endif







   //  Serial.println(ph());
   //   Serial.println(analogRead(phpin));
    //  delay(1000);

     
}


 
