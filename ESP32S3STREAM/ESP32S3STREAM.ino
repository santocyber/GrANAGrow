#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "soc/soc.h"           // Disable brownout problems
#include "sdkconfig.h"
#include "driver/rtc_io.h"
#include <esp_system.h>
#include <nvs_flash.h>
#include "esp_camera.h"
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_attr.h>
#include "esp_heap_caps.h"
#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "mbedtls/base64.h"

// Configuração da câmera
camera_fb_t * fb = NULL;

framesize_t configframesize = FRAMESIZE_XGA; // Tamanho do frame
int framesize = FRAMESIZE_XGA;
int quality = 10;
int qualityconfig = 5;

// Pinos da câmera ESP32-S3-EYE
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5
#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16
#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13

// Credenciais Wi-Fi
const char* ssid = "InternetSA";
const char* password = "cadebabaca";

// URL da API
String serverUrl = "http://52.54.32.237/granagrow/espstream/api.php";

// Variável para armazenar o tempo de upload
long uploadTime = 0;

bool setupCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;  // Para streaming
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;

    if (psramFound()) {
        config.frame_size = configframesize;
        config.jpeg_quality = qualityconfig;
        config.fb_count = 1;  // Use apenas 1 buffer de frame para economizar memória
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 14;
        config.fb_count = 1;
    }

    // Inicializando a câmera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Falha na inicialização da câmera com erro 0x%x\n", err);
        return false;
    } else {
        Serial.println("Câmera inicializada com sucesso.");
    }

    sensor_t * s = esp_camera_sensor_get();
    s->set_framesize(s, (framesize_t)framesize);
    s->set_quality(s, quality);

    // Ajusta brilho (-2 a 2)
    s->set_brightness(s, 0);     

    // Ajusta contraste (-2 a 2)
    s->set_contrast(s, 0);     

    // Ajusta saturação (-2 a 2)
    s->set_saturation(s, 0);  
    s->set_sharpness(s, 0); 

    // Ajusta o ganho automático (0 para desativar, 1 para ativar)
    s->set_gain_ctrl(s, 1);   
    s->set_denoise(s, 1);
    // Ajusta o balanço de branco automático (0 para desativar, 1 para ativar)
    s->set_whitebal(s, 1);    

    // Ajusta a exposição automática (0 para desativar, 1 para ativar)
    s->set_exposure_ctrl(s, 1);

    // Outras configurações possíveis:
     s->set_special_effect(s, 0);   // Efeitos especiais (0 - Sem efeito)
     s->set_awb_gain(s, 0);         // Ganho de AWB
     s->set_wb_mode(s, 0);          // Modo de balanço de branco (0 - Auto)

    // Desativa a correção de lente (Lenc correction)
    s->set_lenc(s, 1);       


    // Ativa espelhamento horizontal e vertical
    s->set_hmirror(s, 0);
    s->set_vflip(s, 0);
    
    s->set_colorbar(s, 0); //Ativa/desativa o padrão de barra de cores para teste. 0 desativa, 1 ativa.


    delay(200);
    return true;
}






void sendImageToServer(void *pvParameters) {
    while (true) {
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            fb = esp_camera_fb_get();
            
            if (!fb) {
                Serial.println("Falha ao capturar imagem");
                return;
            }
                    
                    
            long startTime = millis();


            // Obtenha o endereço MAC correto
            uint8_t mac[6];
            char macStr[18];
            WiFi.macAddress(mac);
            snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", 
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);


            String serverfoto = serverUrl + "?mac_address=" + String(macStr) + "&uploadTime=" + uploadTime;

            http.begin(serverfoto);
    http.addHeader("Content-Type", "image/jpeg");

         // Envia os dados binrios da imagem para o servidor PHP
            Serial.println("TIRANDO FOTO");

    int httpResponseCode = http.POST(fb->buf, fb->len);

    if (httpResponseCode > 0) {
        Serial.printf("[HTTP] POST request done with status code %d\n", httpResponseCode);
        // Obtenha a resposta do servidor como uma string
        String response = http.getString();
        Serial.println("Resposta do servidor: " + response);
    } else {
        Serial.printf("[HTTP] POST request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    
             // Calcula o tempo de upload
            uploadTime = millis() - startTime;
            Serial.printf("Tempo de upload: %ld ms\n", uploadTime);


            // Libere a memória
            esp_camera_fb_return(fb);
            http.end();
        } else {
            Serial.println("Conexão WiFi perdida.");
        }

       
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // Atraso de 1 segundo antes da próxima captura
    }
}


void setup() {
    Serial.begin(115200);

    // Conectar ao Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("WiFi conectado.");

    if (!setupCamera()) {
        Serial.println("Erro ao configurar a câmera.");
    } else {
        Serial.println("Câmera configurada.");
    }

    // Cria a task para envio da imagem
    xTaskCreatePinnedToCore(
        sendImageToServer,    // Função de envio da imagem
        "sendImageTask",      // Nome da tarefa
        8192,                 // Tamanho da stack em bytes
        NULL,                 // Parâmetros para a função
        1,                    // Prioridade da tarefa
        NULL,                 // Handle da tarefa
        1                     // Core para rodar a tarefa
    );
}

void loop() {
    // O loop principal está vazio porque a função de envio está rodando na task
}
