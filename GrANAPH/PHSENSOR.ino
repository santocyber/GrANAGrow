#include <Wire.h>
#include <Adafruit_BMP280.h>  // Biblioteca BMP280
#include <Adafruit_AHTX0.h>   // Biblioteca AHT10
#include <TFT_eSPI.h>         // Biblioteca para o display TFT
#include <SPI.h>
#include <ph4502c_sensor.h>
#include "FS.h"
#include "FFat.h"

// Definindo os pinos I2C para os SENSORES
#define SDA_PIN 21
#define SCL_PIN 47

Adafruit_BMP280 bmp;          // Instância do sensor BMP280
Adafruit_AHTX0 aht;           // Instância do sensor AHT10


#define PH4502C_TEMPERATURE_PIN 47
#define PH4502C_PH_PIN 14
#define PH4502C_CALIBRATION 14.8f
#define PH4502C_READING_INTERVAL 100
#define PH4502C_READING_COUNT 10
#define ADC_RESOLUTION 4096.0f
#define CALIBRATION_TIMEOUT 30000  // 30 segundos de timeout

// Estados para controle da calibração
enum CalibrationState { IDLE, CALIBRATE_PH4, CALIBRATE_PH10 };
CalibrationState calibrationState = IDLE;

float calibrationVoltagePH4 = 1.0;  // Placeholder para voltagem na solução pH 4
float calibrationVoltagePH10 = 2.0;  // Placeholder para voltagem na solução pH 10
float slope, offset;
const char* calibrationFile = "/calibration.txt";  // Nome do arquivo de calibração

unsigned long calibrationStartTime = 0;  // Marca o início da calibração

// Cria uma instância do sensor PH4502C
PH4502C_Sensor ph4502c(
  PH4502C_PH_PIN,
  PH4502C_TEMPERATURE_PIN,
  PH4502C_CALIBRATION,
  PH4502C_READING_INTERVAL,
  PH4502C_READING_COUNT,
  ADC_RESOLUTION
);

float voltage;
float temperature;  
float humidity;     
float pressure;   
float adjusted_ph_level;
float phBruto;  // pH bruto

void setupPH() {
    Wire.begin(SDA_PIN, SCL_PIN);

  // Inicializa o display TFT
  tft.init();
  tft.setRotation(rotate);  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(4);
  
  tft.setCursor(0, 0);
  tft.println("Iniciando...");

  // Inicializa o sensor PH4502C
  ph4502c.init();
  pinMode(PH4502C_PH_PIN, INPUT);



  // Inicializa o sensor AHT10 (umidade e temperatura)
  if (!aht.begin()) {
    Serial.println("Erro ao inicializar o sensor AHT10");
  } else {
    Serial.println("Sensor AHT10 inicializado.");
  }

  // Inicializa o sensor BMP280 (pressão e temperatura)
  if (!bmp.begin()) {  // Endereço padrão do BMP280
    Serial.println("Erro ao inicializar o sensor BMP280");
  } else {
    Serial.println("Sensor BMP280 inicializado.");
  }
  
  // Inicializa o sistema de arquivos FATFS
  if (!FFat.begin()) {
    Serial.println("Erro ao inicializar o FATFS");
    return;
  }

  // Carregar os valores de calibração (voltagens pH 4 e pH 10)
  loadCalibration();

  // Calcular a slope e o offset usando as leituras de pH 4 e pH 10
  calculateSlopeAndOffset();


  delay(500);  
}

void loopPH() {
  phBruto = ph4502c.read_ph_level();  // pH bruto

  // Leitura da temperatura e umidade do AHT10
  sensors_event_t humidity_event, temp_event;
  aht.getEvent(&humidity_event, &temp_event);
  humidity = humidity_event.relative_humidity;
  temperature = temp_event.temperature;

  // Leitura da pressão e temperatura do BMP280
  pressure = bmp.readPressure() / 100.0F;  // Conversão para hPa

  if (calibrationState == IDLE) {
    // Leitura analógica do sensor de pH
    int analogValue = analogRead(PH4502C_PH_PIN);
    voltage = analogValue * (3.3 / ADC_RESOLUTION);

    // Cálculo do pH ajustado usando slope e offset
    adjusted_ph_level = slope * voltage + offset;

    // Exibe os valores no Serial Monitor
    Serial.print("Analog Value: ");
    Serial.print(analogValue);
    Serial.print(" | Voltage: ");
    Serial.print(voltage);
    Serial.print(" | pH Ajustado: ");
    Serial.println(adjusted_ph_level);

    // Exibe os valores na tela
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(3);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("GrANAGrow");
    tft.println("");

    // Exibe pH
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(7);
    tft.print("pH: ");
    tft.println(adjusted_ph_level);  
    tft.println("");

    // Exibe voltagem
    tft.setTextSize(2);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("Voltage:");
    tft.print(voltage);   
    tft.print("|Analog:");
    tft.print(analogValue);
    tft.print("|pH Bruto:");
    tft.print(ph4502c.read_ph_level());
    tft.println("");
    tft.println("");
    tft.setTextColor(TFT_CYAN, TFT_BLACK);

    // Exibe temperatura, umidade e pressão
    tft.print("Temperatura: ");
    tft.println(temperature);
    tft.print("Humidade: ");
    tft.println(humidity);
    tft.print("Pressao: ");
    tft.println(pressure);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, tft.height() - 40);
    tft.print("Toque para iniciar a calibracao");
  } else {
    showCalibrationStatus(); // Mostra os valores durante a calibração
  }

  // Função para detectar o toque
  handleTouch();
}

// Exibe os valores durante a calibração
void showCalibrationStatus() {
  int analogValue = analogRead(PH4502C_PH_PIN);
  voltage = analogValue * (3.3 / ADC_RESOLUTION);

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  
  if (calibrationState == CALIBRATE_PH4) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibracao pH 4");

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(4);
    tft.print("Voltage: ");
    tft.println(voltage);

    tft.setTextSize(2);
    tft.println("Toque para confirmar");

  } else if (calibrationState == CALIBRATE_PH10) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibracao pH 10");

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(4);
    tft.print("Voltage: ");
    tft.println(voltage);

    tft.setTextSize(2);
    tft.println("Toque para confirmar");
  }

  // Verifica se o tempo limite foi atingido
  if (millis() - calibrationStartTime > CALIBRATION_TIMEOUT) {
    Serial.println("Tempo de calibração expirado. Voltando para a tela inicial.");
    calibrationState = IDLE;
  }
}

// Função para calcular a slope e o offset
void calculateSlopeAndOffset() {
  slope = (10.0 - 4.0) / (calibrationVoltagePH10 - calibrationVoltagePH4);  // (pH10 - pH4) / (V10 - V4)
  offset = 4.0 - (slope * calibrationVoltagePH4);  // pH4 - slope * V4

  Serial.print("Slope: ");
  Serial.println(slope);
  Serial.print("Offset: ");
  Serial.println(offset);
}

// Função para salvar o valor de calibração no sistema de arquivos
void saveCalibration() {
  File file = FFat.open(calibrationFile, FILE_WRITE);
  if (file) {
    file.println(calibrationVoltagePH4);
    file.println(calibrationVoltagePH10);
    file.close();
    Serial.println("Calibração salva no arquivo.");
  } else {
    Serial.println("Erro ao salvar o arquivo de calibração.");
  }
}

// Função para carregar os valores de calibração do sistema de arquivos
void loadCalibration() {
  File file = FFat.open(calibrationFile);
  if (file) {
    String ph4Value = file.readStringUntil('\n');
    calibrationVoltagePH4 = ph4Value.toFloat();

    String ph10Value = file.readStringUntil('\n');
    calibrationVoltagePH10 = ph10Value.toFloat();

    file.close();
    Serial.print("Calibração carregada do arquivo: pH4 = ");
    Serial.print(calibrationVoltagePH4);
    Serial.print(", pH10 = ");
    Serial.println(calibrationVoltagePH10);
  } else {
    Serial.println("Nenhum arquivo de calibração encontrado. Usando valores padrão.");
  }
}

void handleTouch() {
  uint16_t x, y;
  if (tft.getTouch(&x, &y)) {  
    calibrationStartTime = millis();  // Reinicia o timer ao detectar o toque

    if (calibrationState == IDLE) {
      // Começar a calibração
      calibrationState = CALIBRATE_PH4;
      calibrationStartTime = millis();  // Marca o tempo do início da calibração
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Submerja o sensor em solucao pH 4");
      tft.println("Aguarde leitura e toque para confirmar");
      delay(1000);
    } else if (calibrationState == CALIBRATE_PH4) {
      // Capturar a voltagem da solução de pH 4
      int analogValue = analogRead(PH4502C_PH_PIN);
      calibrationVoltagePH4 = analogValue * (3.3 / ADC_RESOLUTION);
      Serial.print("Voltagem pH 4 capturada: ");
      Serial.println(calibrationVoltagePH4);

      // Passar para o próximo passo (calibração de pH 10)
      calibrationState = CALIBRATE_PH10;
      calibrationStartTime = millis();  // Reinicia o timer para o pH 10
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Submerja o sensor em solucao pH 10");
      tft.println("Aguarde leitura e toque para confirmar");
    } else if (calibrationState == CALIBRATE_PH10) {
      // Capturar a voltagem da solução de pH 10
      int analogValue = analogRead(PH4502C_PH_PIN);
      calibrationVoltagePH10 = analogValue * (3.3 / ADC_RESOLUTION);
      Serial.print("Voltagem pH 10 capturada: ");
      Serial.println(calibrationVoltagePH10);

      // Calcular slope e offset e salvar
      calculateSlopeAndOffset();
      saveCalibration();

      // Voltar ao estado inicial (IDLE)
      calibrationState = IDLE;
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Calibracao completa!");
      delay(2000);  // Esperar um momento antes de voltar à tela inicial
    }
  }
}
