#include <Wire.h>
#include <Adafruit_BMP280.h>  // Biblioteca BMP280
#include <Adafruit_AHTX0.h>   // Biblioteca AHT10
#include <TFT_eSPI.h>         // Biblioteca para o display TFT
#include <SPI.h>

// Definindo os pinos I2C para os SENSORES
#define SDA_PIN 21
#define SCL_PIN 47

Adafruit_BMP280 bmp;          // Instância do sensor BMP280
Adafruit_AHTX0 aht;           // Instância do sensor AHT10

// Função de estilo para exibir os dados com layout
void displayData(const char* label, float value, uint16_t x, uint16_t y, uint16_t color, uint8_t textSize) {
  tft.setCursor(x, y);
  tft.setTextSize(textSize);
  tft.setTextColor(color, TFT_BLACK); // Cor personalizada
  tft.print(label);
  tft.println(value, 1); // Valor com 1 casa decimal
}

void setupCLIMA() {
  // Inicializa o Wire com os pinos I2C definidos
  Wire.begin(SDA_PIN, SCL_PIN);

  // Inicializa o display TFT
  tft.init();
  tft.setRotation(rotate); // Define a rotação, ajuste conforme necessário
  tft.fillScreen(TFT_BLACK);

  // Carregar a fonte personalizada
  tft.loadFont("NotoSansBold15"); // Verifique se você tem a fonte disponível

  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Texto branco com fundo preto
  tft.println("Iniciando...");

  // Inicializa o BMP280 com o Wire personalizado
  if (!bmp.begin()) {  // Verifique o endereço I2C do seu BMP280 (0x76 ou 0x77)
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Erro no BMP280!");
    while (1); // Para o código se o sensor BMP280 falhar
  }

  // Inicializa o AHT10 com o Wire personalizado
  if (!aht.begin()) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.println("Erro no AHT10!");
    while (1); // Para o código se o sensor AHT10 falhar
  }

  // Configura o BMP280 para leitura de pressão
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,   // Oversampling temperatura
                  Adafruit_BMP280::SAMPLING_X16,  // Oversampling pressão
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  delay(500); // Pequena pausa para mostrar a mensagem inicial
}

void loopCLIMA() {
  sensors_event_t ahtHumidity, ahtTemp;
  aht.getEvent(&ahtHumidity, &ahtTemp); // Lê os eventos de umidade e temperatura do AHT10

  // Lê os valores de temperatura e pressão do BMP280
   temperature = bmp.readTemperature();       // Temperatura do BMP280 (pode optar pela do AHT10)
   pressure = bmp.readPressure() / 100.0F;    // Pressão em hPa
   humidity = ahtHumidity.relative_humidity;  // Umidade relativa do AHT10

  // Limpa a tela e estiliza a amostragem
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); // Texto verde para o título
  tft.setCursor(10, 20);
  tft.println("GrANAGrow");

  // Estiliza a exibição dos valores com layout e cores
  displayData("TEMPERATURA: ", temperature, 10, 100, TFT_CYAN, 4);
  displayData("UMIDADE: ", humidity, 10, 160, TFT_YELLOW, 4);
  displayData("PRESSAO: ", pressure, 10, 220, TFT_ORANGE, 4);

  delay(5500); // Pausa para evitar atualizações muito rápidas
}
