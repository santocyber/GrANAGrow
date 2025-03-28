#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// ================= Configurações =================
// WiFi
const char* WIFI_SSID = "InternetSA";
const char* WIFI_PASS = "cadebabaca";

// Servidor
const char* SERVER_URL = "http://52.54.32.237/granagrow/espstream/api.php";
const int UPLOAD_INTERVAL = 60000; // 5 segundos

// Câmera
#define FRAMESIZE FRAMESIZE_SVGA  // 800x600
#define JPEG_QUALITY 10
#define FLASH_GPIO 4

// ============== Pinagem da Câmera ================
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// ================== Protótipos ====================
bool setupCamera();
bool connectWiFi();
void sendImageTask(void *pvParameters);
String getMacAddress();

// ================== Variáveis ====================
camera_fb_t *fb = NULL;

void setup() {
  Serial.begin(115200);
  pinMode(FLASH_GPIO, OUTPUT);
  
  // 1. Conectar WiFi
  if(!connectWiFi()) {
    Serial.println("Falha WiFi! Reiniciando...");
    ESP.restart();
  }

  // 2. Inicializar Câmera
  if(!setupCamera()) {
    Serial.println("Falha Câmera! Reiniciando...");
    ESP.restart();
  }

  // 3. Iniciar Task de Envio
  xTaskCreatePinnedToCore(
    sendImageTask,
    "sendImageTask",
    8192,
    NULL,
    1,
    NULL,
    1
  );
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}

// ============= Implementação das Funções ==============
bool setupCamera() {
  camera_config_t config;
  
  // Configuração do LEDC
  config.ledc_channel = LEDC_CHANNEL_1;
  config.ledc_timer = LEDC_TIMER_2;
  
  // Configuração dos pinos
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;

  // Parâmetros da câmera
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE;
  config.jpeg_quality = JPEG_QUALITY;
  config.fb_count = 1;
  config.fb_location = CAMERA_FB_IN_PSRAM;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Erro câmera: 0x%x\n", err);
    return false;
  }
  return true;
}

bool connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    Serial.print(".");
    delay(500);
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFalha na conexão!");
    return false;
  }
  
  Serial.println("\nConectado! IP: " + WiFi.localIP().toString());
  return true;
}

String getMacAddress() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

void sendImageTask(void *pvParameters) {
  while (true) {
    // 1. Verificar conexão
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Reconectando WiFi...");
      WiFi.reconnect();
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      continue;
    }

    // 2. Capturar imagem
    digitalWrite(FLASH_GPIO, HIGH);
    fb = esp_camera_fb_get();
    digitalWrite(FLASH_GPIO, LOW);
    
    if (!fb) {
      Serial.println("Falha na captura!");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }

    // 3. Preparar requisição
    HTTPClient http;
    String url = String(SERVER_URL) + 
                "?mac_address=" + getMacAddress() + 
                "&uploadTime=" + millis();

    http.begin(url);
    http.addHeader("Content-Type", "image/jpeg");
    http.setTimeout(10000);

    // 4. Enviar imagem
    Serial.printf("Enviando %d bytes...", fb->len);
    int httpCode = http.POST(fb->buf, fb->len);

    // 5. Processar resposta
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.printf("Sucesso! Resposta: %s\n", response.c_str());
    } else {
      Serial.printf("Erro HTTP: %d\n", httpCode);
    }

    // 6. Limpar recursos
    http.end();
    esp_camera_fb_return(fb);
    fb = NULL;

    vTaskDelay(UPLOAD_INTERVAL / portTICK_PERIOD_MS);
  }
}
