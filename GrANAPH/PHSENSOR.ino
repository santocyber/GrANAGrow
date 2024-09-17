
#include <ph4502c_sensor.h>

/* Pinout: https://cdn.awsli.com.br/969/969921/arquivos/ph-sensor-ph-4502c.pdf */
#define PH4502C_TEMPERATURE_PIN 47
#define PH4502C_PH_PIN 14
#define PH4502C_PH_TRIGGER_PIN 41 
#define PH4502C_CALIBRATION 14.8f
#define PH4502C_READING_INTERVAL 100
#define PH4502C_READING_COUNT 10
// NOTE: The ESP32 ADC has a 12-bit resolution (while most arduinos have 10-bit)
#define ADC_RESOLUTION 4096.0f


// Create an instance of the PH4502C_Sensor
PH4502C_Sensor ph4502c(
  PH4502C_PH_PIN,
  PH4502C_TEMPERATURE_PIN,
  PH4502C_CALIBRATION,
  PH4502C_READING_INTERVAL,
  PH4502C_READING_COUNT,
  ADC_RESOLUTION
);

float voltage;
float phValue;
float temperature; // Placeholder para temperatura, ajustar conforme sua leitura
float humidity;    // Placeholder para umidade, ajustar conforme sua leitura
float pressure;  // Placeholder para pressão, ajustar conforme sua leitura
float Offset = 0.0; // Defina o offset inicial (ajuste conforme necessário)

void setupPH() {
  // Inicializa o display TFT
  tft.init();
  tft.setRotation(rotate); // Define a rotação, ajuste conforme necessário
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Texto branco com fundo preto
  tft.setTextSize(4);

  tft.setCursor(0, 0);
  tft.println("Iniciando...");
  // Initialize the PH4502 instance
  ph4502c.init();
  pinMode(PH4502C_PH_PIN, INPUT);

  delay(500); // Pequena pausa para mostrar a mensagem inicial
}




void loopPH() {
  int analogValue = analogRead(PH4502C_PH_PIN);
  voltage = analogValue * (3.3 / ADC_RESOLUTION); // ESP32 tem resolução ADC de 12 bits
  phValue = 3.5 * voltage + Offset; // Calibração simples, ajuste o Offset conforme necessário
  
  // Exibe os valores no Serial Monitor
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage);

  // Limpa a tela e exibe os valores
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(6);

  tft.setTextColor(TFT_GREEN, TFT_BLACK); // Texto branco com fundo preto
  tft.println("GrANAGrow");  
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Texto branco com fundo preto
  tft.println("");

   tft.setTextSize(5);

  tft.print("pH: ");
  tft.println(ph4502c.read_ph_level());
  tft.println(ph4502c.read_ph_level_single());
  tft.setTextSize(4);

  tft.println("");
  tft.setTextColor(TFT_CYAN, TFT_BLACK); // Texto branco com fundo preto

  tft.print("ADC: ");
  tft.println(analogValue);
  tft.print("Voltage: ");
  tft.println(voltage);
  tft.print("Ph old: ");
  tft.println(phValue);

  // Substitua os placeholders com leituras reais
  temperature = 25.0; 
  humidity = 50.0;    
  pressure = 1013.0;  

  //delay(1500);
}
