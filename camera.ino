
#if (CAMERA == 1)
//####################CONFIG CAMERA

#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "soc/soc.h"           // Disable brownour problems
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

//#########################CAMERA

// Configurao da cera
camera_fb_t * fb = NULL;

framesize_t configframesize = FRAMESIZE_SXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
int framesize = FRAMESIZE_HD; //FRAMESIZE_HD;
int quality = 8;
int qualityconfig = 4;


#define ESP32S3 1
#define ESP32W  0



#if (ESP32S3 == 1)

/// defined(CAMERA_MODEL_ESP32S3_EYE)
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

#endif

#if (ESP32W == 1)

//#if defined(CAMERA_MODEL_WROVER_KIT)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22
#endif



bool setupCamera()
{
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
 // config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  //config.jpeg_quality = 12;
  //config.fb_count = 1;
  
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = configframesize;
    config.jpeg_quality = qualityconfig;
    config.fb_count = 4; 
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());

  static char * memtmp = (char *) malloc(32 * 1024);
  static char * memtmp2 = (char *) malloc(32 * 1024); //32767

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return false;
  }
 
  free(memtmp2);
  memtmp2 = NULL;
  free(memtmp);
  memtmp = NULL;


  sensor_t1 * s = esp_camera_sensor_get();
  //  drop down frame size for higher initial frame rate
  s->set_framesize(s, (framesize_t)framesize);
  s->set_quality(s, quality);
  delay(200);
  return true;
}









//###############################################TIRA FOTO ENVIA

void capturaimagem() {
    Serial.println("INICIA FUNCAO AUTO CAMERA");

    timerfotostatus.trim();

    if (timerfotostatus == "1") {
    Serial.println("Modo FOTO automático ativado");
    capturaimagemenviabd();
 
}else{
      Serial.println("Modo FOTO automático desativado");

  }

}


void capturaimagemenviabd() {
            Serial.println("TIRANDO FOTO");

    // Limpa o buffer da câmera antes de enviar a foto para a API
    esp_camera_fb_return(fb);

        // Captura da foto
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Erro ao capturar a foto");
        return;
    }
    
delay(400);

        // Limpa o buffer da câmera antes de enviar a foto para a API
    esp_camera_fb_return(fb);

        // Captura da foto
    esp_camera_fb_get();
    if (!fb) {
        Serial.println("Erro ao capturar a foto");
        return;
    }

    // Certifique-se de que o valor do MAC seja fornecido corretamente
    String urlx = urlsalvafoto + "?mac=" + WiFi.macAddress();
    
    // Inicialize uma conexo HTTPClient
    HTTPClient http;
   // http.begin(client,urlx);
    http.begin(urlx);
    http.addHeader("Content-Type", "image/jpeg");

    Serial.println("Enviando foto para a API BD...");
    

    int httpResponseCode = http.POST((uint8_t*)fb->buf, fb->len);

    if (httpResponseCode > 0) {
        Serial.print("Resposta da API: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.print("Resposta da mensagem: ");
        Serial.println(response);
    } else {
        Serial.print("Erro ao enviar a foto. Código de erro: ");
        Serial.println(httpResponseCode);
        Serial.print("Motivo: ");
        Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    // Limpa o buffer da câmera após o envio da foto para a API
    esp_camera_fb_return(fb);
    http.end();
}

#endif
