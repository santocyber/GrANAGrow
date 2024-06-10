//###Green Anarchy project GrANAGROW
//###by SantoCyber
//###segue no YT @RastaNerdi
//###
//###



//###CONFIG INCIAIS ATIVACAO DE MODULOS

#define SENSORES 1
#define PH 0
#define CAMERA 0
#define PIR 0
#define TELA 1
#define SDCARD 0
#define RELES 0



//#Bibliotecas 

//#Bibliotecas REDE
#include <WiFi.h>
//#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ESPping.h>
//#include <WiFiClientSecure.h>

//#Biblioteca WEBSERVER
#include <WebServer.h>

// Cria um objeto WebServer
WebServer server(80);

//#Biblioteca SISTEMAS
#include <esp_task_wdt.h>
#include "time.h"
#include "esp_sntp.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <stdbool.h>
#include <stdint.h>
#include <SPI.h>
#include "Arduino.h"

#if (TELA == 1)

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#endif

//#Biblioteca SENSORES
#if (SENSORES == 1)

#include <Adafruit_BMP280.h>  
#include <Adafruit_AHTX0.h> 

Adafruit_BMP280 bmp;  // Objeto do sensor BMP280
Adafruit_AHTX0 aht;   // Objeto do sensor AHTX0
//#define BMP280_ADDRESS 0x77
//#define AHT_ADDRESS 0x38

Adafruit_Sensor *aht_humidity, *aht_temp;
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

#endif


//###################################GRANA VERSION

String GRANAVERSION = "0.16";

//##################### Configura IP
//WiFiClientSecure client;

IPAddress localIP;
IPAddress localGateway;

//IPAddress localIP(192, 168, 1, 200); // hardcoded
//IPAddress localGateway(192, 168, 1, 1); //hardcoded

IPAddress subnet(255, 255, 255, 0);
IPAddress dns1(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns meu roteador nao estava liberando o dns
IPAddress dns2(9, 9, 9, 9); 
//IPAddress dns2(65, 19, 143, 3); 
//heliohost dns ip 65.19.143.3

// Configuração do certificado CA (Certificado de Autoridade) para confiar no servidor remoto
const char* caCert = \
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




//##############CONFIG TASKHANDLE
// Define o tamanho da pilha para ambas as tarefas.
#define STACK_SIZE 5000

// Define os identificadores de tarefa.
TaskHandle_t xHandle_TELE = NULL;
TaskHandle_t xHandle_CAM = NULL;

// Define os buffers e pilhas de tarefa estáticos.
StaticTask_t xTaskBuffer_TELE, xTaskBuffer_CAM;
StackType_t xStack_TELE[STACK_SIZE], xStack_CAM[STACK_SIZE];

//###################################CONFIGURA VARIAVIS

//###########################CONFIGURACAO DAS PINAGENS


#if (PH == 1)

#define phpin 1

#endif
#if (PIR == 1)

#define PIR_PIN 42

#endif
#if (RELES == 1)

#define RELAY1_PIN 27
#define RELAY2_PIN 33
#define RELAY3_PIN 25

#endif

#if (SENSORES == 1)

//#define I2C_MASTER_SDA1 20
//#define I2C_MASTER_SCL1 21

#define I2C_MASTER_SCL1 25
#define I2C_MASTER_SDA1 26

#endif



//#define I2C_MASTER_SCL2 5 // Exemplo de pino para a segunda porta I2C
//#define I2C_MASTER_SDA2 4 // Exemplo de pino para a segunda porta I2C

const int Button1 = 0;  
const int ledPin = 0;    

const char* host = "santocyber.helioho.st";
String site =        "http://santocyber.helioho.st/";
String url =        "http://santocyber.helioho.st/granagw/gravatele.php";
String urlsalvafoto = "http://santocyber.helioho.st/granagw/salvafoto.php";
String urlverifica = "http://santocyber.helioho.st/granagw/verificaacao.php";
String qrdoa =        "00020101021226900014BR.GOV.BCB.PIX2568pix-qr.mercadopago.com/instore/p/v2/dbb558faf3c14a7a98e7e8fb805f6c1643530016com.mercadolibre0129https://mpago.la/pos/162635835204000053039865802BR5922Cooperativa mirako org6009SAO PAULO62070503***6304C530";
String urlchat =        "http://santocyber.helioho.st/granagw/lermsgchat.php";
String urlbolsa =        "http://santocyber.helioho.st/money/indexesp.php";
String urlfototft =        "http://santocyber.helioho.st/granagw/fototft.php";

const String rss_feed_url = "https://feeds.bbci.co.uk/portuguese/rss.xml";


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

int rotate = 3;


String horaAtual;
String dataAtual;

String StateUpdate = "desativado";
String payload;
String payloadx;
String payloadxx;
String pagamento;
String estado;
String mensagem;
String telastatus;
String mensagemstatus;
String hrliga;
String hrdesliga;
String timerautomatico;
String timerfotostatus;
String timerfoto = "10";
String statetela;
String statetoque;
String msgtg;
String statepir = "0";
String statetg = "0";
String chat_id;


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
int contadorpix = 0;


// Vari         veis para rastrear o estado do bot         o e os cliques
int objState = 0;       // Vari         vel para armazenar o estado do bot         o
int contaobjfunc = 0;
bool functionExecuted = false;
bool Executed = true;
bool buttonsCreated = false; // Variável para controlar se os botões foram criados
bool stopFetching = true;
bool boolpix = true;
bool boolbotao = true;
bool boololhos = true;

int sensorValue = 0;
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
unsigned long previousMillis4 = 0;

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

const int buttonPin = 0; 
bool buttonState = false; 
bool lastButtonState = false; 
unsigned long startTime = 0; 
const unsigned long longPressDuration = 10000; 




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
       Serial.setDebugOutput(true);

        // esp_task_wdt_deinit();
//  esp_task_wdt_init(240, true); //enable panic so ESP32 restarts
 // esp_task_wdt_add(NULL); //add current thread to WDT watch

  
   Serial.println("Iniciando dispositivo GRANA");
   Serial.println("");
      Serial.println("Iniciando CONFIGURACAO DE PINAGEM");

#if (PH == 1)
  // Configurar o pino do PH
  pinMode(phpin, INPUT); // Define o pino do bot como entrada
  #endif
#if (PIR == 1)
  // Configurar o pino do Pir
  pinMode(PIR_PIN, INPUT);

  #endif
  #if (RELES == 1)

    // Configurando os pinos dos relés como saída
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);

  // Inicializando os relés desligados
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  #endif

  pinMode(buttonPin, INPUT_PULLUP);
  

    
#if (SENSORES == 1)
   Serial.println("Iniciando SENSORES");


  // Inicialize o barramento I2C com os pinos SDA e SCL 
 // Wire.begin(); // Inicialize a primeira porta I2C  
 // Wire.begin(I2C_MASTER_SDA2, I2C_MASTER_SCL2); // Inicialize a segunda porta I2C
 // Wire.setClock(100000); 
    Wire.begin(I2C_MASTER_SDA1, I2C_MASTER_SCL1); // Inicialize a primeira porta I2C

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


   Serial.println("Iniciando SPIFF");


if (!SPIFFS.begin(true)) {
    Serial.println("ERRO NO SPIFFS");
  }
  Serial.println("SPIFFS MONTADO");

  last_id = read_nvs_data();


// Carrega as credenciais Wi-Fi e realiza trim nos valores
loadWifiCredentials(ssid, password, nomedobot, geo, usuario);
ssid.trim();
password.trim();
nomedobot.trim();

Serial.println("SSID: " + ssid);
Serial.println("Password: " + password);
Serial.println("Nome do bot: " + nomedobot);
Serial.println("Usuário: " + usuario);

   Serial.println("Iniciando WIFI");

// Verifica se tanto o SSID quanto a senha não estão vazios
if (ssid.length() > 0 && password.length() > 0) {
    // Tenta conectar-se à rede Wi-Fi
    WiFi.config(IPAddress(), dns1, dns2);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.println("Conectando ao WiFi...");
   // client.setInsecure();//skip verification

    // Aguarda conexão por até 10 segundos
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
    }
    // Se não conectar, iniciar o modo AP
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Falha ao reconectar ao Wi-Fi.");
        startAPMode();
        delay(500);
       
    } else {
        Serial.println("Conexão bem-sucedida, iniciar o servidor web e o update ativo");

        // Conexão bem-sucedida, iniciar o servidor web
        StateUpdate = "ativo";
        startWebServer();
    }
} else {
    // Se as credenciais estiverem vazias, iniciar o modo AP
    startAPMode();
}
    Serial.println("Config do ESP finalizada");

    
#if (TELA == 1)

   Serial.println("iniciando tela");
    configfototft();
    tft.init();
    tft.setRotation(rotate);


     // Se não conectar, iniciar o modo AP
    if (WiFi.status() != WL_CONNECTED) {
  tft.fillScreen(TFT_RED);
  tft.setTextSize(5);
  tft.setCursor(0, 0); // Set cursor at top left of screen
  tft.setCursor(10, 10);  // Posição do cursor na tela
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Change the font colour and the background colour
  tft.println("SEM CONEXAO");
  delay(2000);
  scanWiFiNetworks();
        
    } else {
        configwifi();
    }
    
    menu();

#endif


  loadfile(estado, telastatus, mensagemstatus, mensagem);
  loadTime(hrliga, hrdesliga, timerautomatico, timerfoto, timerfotostatus);

  
    Serial.println("VARIAVEIS ARMAZENADA NO SPIFF");
    Serial.println(estado);
    Serial.println(mensagem);
    Serial.println(mensagemstatus);
    Serial.println(telastatus);

    Serial.println(hrliga);
    Serial.println(hrdesliga);
    Serial.println(timerautomatico);
    Serial.println(timerfotostatus);
    Serial.println(timerfoto);


//#####################################################wifi scan

#if (CAMERA == 1)
    // Camera init

    Serial.println("Iniciando config CAM");
if (!setupCamera())
  {
    Serial.println("Camera Setup Failed!");
  }else{    Serial.println("Camera INICIADA!");
}
#endif


// Create the first task pinned to core 0.
xHandle_TELE = xTaskCreateStaticPinnedToCore(
    TELE,            // Function that implements the task.
    "TELE",          // Text name for the task.
    STACK_SIZE,      // Stack size in bytes, not words.
    (void *)1,       // Parameter passed into the task. (void *)1
    4,               // Priority at which the task is created.
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


   Serial.println("Dispositivo GRANA iniciado!");

  }


void loop() {

    esp_task_wdt_reset();

    server.handleClient();

  unsigned long currentMillis = millis();

#if (TELA == 1)

if (currentMillis - previousMillis3 >= 1000) {
    // Atualiza o tempo de referencia
    previousMillis3 = currentMillis;
    tela();

}

     if (currentMillis - previousMillis2 >= 300) {
    // Atualiza o tempo de referencia
    previousMillis2 = currentMillis;
    toque();

}  

#endif



// Verificar se passaram 30 segundos
if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;

    //  telemetria();
    //  verifyActionAndExecute();
                   // sendPhotoToAPI();



/*


                      Serial.println("PH");
   Serial.println(digitalRead(phpin));
   Serial.println(analogRead(phpin));

   
   Serial.println("PIR");
   Serial.println(digitalRead(PIR_PIN));
   Serial.println(analogRead(PIR_PIN));
   */

   

#if (PIR == 1)

 if (statepir == "1") {


pir();
  }
#endif

    }






}


 
