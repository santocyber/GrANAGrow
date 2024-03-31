
#include <stdbool.h>
#include <stdint.h>
#include "qrcodegen.h"

//char buffer[300];
uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];


String urlpix = "https://santocyber.helioho.st/pix/pixapi.php";
String urlorders = "https://santocyber.helioho.st/pix/api_orders.php";





//botao na tela

const int buttonSize = 75;
const int buttonMargin = 10;
const int screenWidth = tft.width();
const int screenHeight = tft.height();
const int buttonPlusX = screenWidth - buttonSize - buttonMargin;
const int buttonPlusY = screenHeight - buttonSize - buttonMargin - buttonMargin;
const int buttonMinusX = screenWidth - buttonSize - buttonMargin;
const int buttonMinusY = screenHeight - buttonSize - buttonMargin;
const int buttonSumX = screenWidth - buttonSize - buttonMargin;
const int buttonSumY = buttonMargin;
const int buttonSumSize = 75;


unsigned long lastInteractionTime = 0;
bool isAdding = false;









void pix() {
  last_id = read_nvs_data();
  last_id++;
  save_nvs_data(last_id);
  Serial.print("Ultima Compra: ");
  Serial.println(last_id);

  HTTPClient http;

  // Construir o corpo da solicitação POST
  String body = "external_id=" + String(last_id) +
                "&value=" + String(ordervalue) +
                "&mac=" + WiFi.macAddress();

  http.begin(urlpix);  // Use apenas a URL base, sem parâmetros

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  //neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // Green

  int maxAttempts = 10; // Número máximo de tentativas
  bool success = false;
  int httpCode; // Declare a variável httpCode aqui

  for (int attempt = 1; attempt <= maxAttempts; attempt++) {
      delay(500);
    Serial.printf("[HTTPS] Tentativa %d...\n", attempt);

    // Fazer a solicitação POST
    httpCode = http.POST(body); // Atribua o valor a httpCode e envie o corpo da solicitação

    if (httpCode == HTTP_CODE_OK) {
      success = true;
      break; // Conexão bem-sucedida, saia do loop
    }

    Serial.printf("[HTTPS] Tentativa %d falhou, código: %d\n", attempt, httpCode);
    //ensureWiFiConnection();

    delay(1000);
  }

  if (success) {
    Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

    payloadx = http.getString();
    Serial.println(payloadx);
    Serial.println(urlpix);
    Serial.println("QR CODE FUNÇÃO");
    Serial.print("VALOR:");
    Serial.println(ordervalue);
    Serial.println("QR CODE:");
    Serial.println(payloadx.c_str());
    Serial.println("Gerando QR Code...");
    Serial.println("ESPERANDO PAGAMENTO...");
    qrcodefun();
    statetela = "";

    delay(5000);
    orders();


  } else {
    Serial.println(F("Número máximo de tentativas atingido, não foi possível estabelecer a conexão."));
    statetela = "olhos";

  }

 // neopixelWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS); // Blue
  delay(1000);
  Serial.println("");
  Serial.println(F("Desconectando."));
  http.end();

}



void print_qrcode(const uint8_t qrcode[]) {
   tft.setRotation(3);

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0); // Set cursor at top left of screen
  
  int size = qrcodegen_getSize(qrcode);
  char border = 4;
  char module_size = 4;
  uint16_t color;
  int xOffset = 86;
  int yOffset = 6;

  for (int y = -border; y < size + border; y++) {
    for (int x = -border; x < size + border; x++) {
      color = qrcodegen_getModule(qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      int pixelX = (x + border) * module_size + xOffset;
      int pixelY = (y + border) * module_size + yOffset;

      for (int i = 0; i < module_size; i++) {
        for (int j = 0; j < module_size; j++) {
          tft.drawPixel(pixelX + j, pixelY + i, color);
        }
      }
    }
  }
}


void qrcodefun() {

  Serial.println("QR CODE: " + payloadx);
  Serial.println("Gerando QR Code...");

  uint8_t* qrcode = (uint8_t*)malloc(qrcodegen_BUFFER_LEN_MAX);
  uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];

  bool ok = qrcodegen_encodeText(payloadx.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

  if (ok) {
    print_qrcode(qrcode);
  }

  free(qrcode); // Liberar a memória alocada dinamicamente
  Serial.println("QR Code gerado e Impresso");
}








void orders() {
  //ensureWiFiConnection();
  HTTPClient http;
  int httpResponseCode;
  int maxTries = 40;
  
  for (verificacoes = 0; verificacoes < maxTries; verificacoes++) {
       Serial.printf("[HTTPS] Tentativa %d...\n", verificacoes);

    String postDataStr;
    postDataStr.reserve(100); // Reserve espaço para evitar realocações frequentes

    postDataStr = "external_id=" + String(last_id) + "&mac=" + WiFi.macAddress();

  http.begin(urlorders);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    postDataStr.trim();
    httpResponseCode = http.POST(postDataStr);
    Serial.println("Enviando POST: " + postDataStr);
    Serial.println("URL: " + urlorders);

    if (httpResponseCode == HTTP_CODE_OK) {
      String payloadxx = http.getString();
      payloadxx.trim();
      Serial.println("Resposta da API: " + payloadxx);
      Serial.println("QR CODE:");
      Serial.println(payloadx.c_str());

      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payloadxx);
      if (!error) {
          pagamento = doc["order_status"].as<String>();
          String externalid = doc["external_id"];
          Serial.println("Status do pedido: " + pagamento);
          Serial.println("ID SQL: " + externalid);
          Serial.println("ID ROM: " + String(last_id));

          if (pagamento == "1") {


  tft.fillScreen(TFT_GREEN);  // Limpa a tela com fundo verde
  tft.setTextColor(TFT_WHITE);  // Define a cor do texto como branco
  tft.setTextSize(6);  // Define o tamanho do texto

  tft.setCursor(50, 100);  // Posição do cursor na tela
  tft.println("PAGAMENTO");
  tft.println("EFETUADO");
  tft.println("");
  tft.println("OBRIGADO"); 
  tft.println("DEUS TE ABENCOE!");
            //acionabraco();
    statetela = "";


  delay(10000);
     statetela = "olhos";

            break;
          } else if (pagamento == "0") {
            // Não é necessário incrementar verificacoes neste caso.
          Serial.println("pagamento nao efetuado verificado no sql");

          }
         
      } else {
        Serial.println("Erro ao analisar resposta: " + String(error.c_str()));
      }
    } else {
      Serial.println("Erro HTTP: " + String(httpResponseCode));
     // ensureWiFiConnection();

    }

    Serial.println("");
    Serial.println(F("Desconectando."));
    http.end();
    delay(1000);

    
    }



    
}





//botao na tela

#define TFT_GREY 0x5AEB // Cor cinza para os botões
#define TFT_WHITE 0xFFFF // Cor branca para o texto

#define BUTTON_WIDTH 60 // Largura dos botões
#define BUTTON_HEIGHT 60 // Altura dos botões




void drawButtons() {
  tft.fillScreen(TFT_BLACK);
  // Desenhando o botão "Menos"
  drawButton(50, 170, "-", TFT_RED);

  // Desenhando o botão "Mais"
  drawButton(140, 170, "+", TFT_GREEN);
  
  // Mostrar valor atual
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(6);
  tft.setCursor(10, 10);
  tft.print("Valor: " + String(ordervalue));
  functionExecuted = true;  // Marca a função como executada

}

// Função para verificar qual botão foi pressionado
void checkButtons(int x, int y) {
       functionExecuted = false;  // Marca a função como executada

        loopCounter = 0; // Reseta o contador de loop

  if (x < BUTTON_WIDTH && y > 100 && y < 100 + BUTTON_HEIGHT) {
    ordervalue--; // Diminuindo o valor
    displayValue(); // Atualizando o display
  }
  else if (x > 100 && y > 100 && y < 100 + BUTTON_HEIGHT) {
    ordervalue++; // Aumentando o valor
    displayValue(); // Atualizando o display
  }
}

// Função para desenhar um botão na tela
void drawButton(int x, int y, String label, uint16_t color) {
  tft.fillRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, color); // Desenhando o botão
  tft.drawRect(x, y, BUTTON_WIDTH, BUTTON_HEIGHT, TFT_WHITE); // Desenhando a borda do botão
  tft.setTextColor(TFT_WHITE); // Configurando a cor do texto
  tft.setTextSize(2); // Configurando o tamanho do texto
  tft.setTextDatum(MC_DATUM); // Configurando o ponto de referência do texto
  tft.drawString(label, x + BUTTON_WIDTH / 2, y + BUTTON_HEIGHT / 2); // Exibindo o texto no centro do botão
}

// Função para exibir o valor na tela
void displayValue() {
  tft.fillRect(50, 50, 100, 30, TFT_BLACK); // Limpando a área onde o valor é exibido
  tft.setTextColor(TFT_WHITE); // Configurando a cor do texto
  tft.setTextSize(2); // Configurando o tamanho do texto
  tft.setTextDatum(MC_DATUM); // Configurando o ponto de referência do texto
  tft.drawString(String(ordervalue), 100, 65); // Exibindo o valor no centro da tela
}
