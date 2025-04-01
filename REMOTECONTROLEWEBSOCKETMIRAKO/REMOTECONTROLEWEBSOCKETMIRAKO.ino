#include <WiFi.h>
#include <WiFiMulti.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <WebSocketsClient_Generic.h>

// ===================== Configurações WiFi =====================
WiFiMulti wifiMulti;
const char* ssid = "InternetSA";
const char* password = "cadebabaca";

// ===================== Configurações BLE =====================
const char* targetName = "Wireless Controller";  // Nome do controle
BLEAdvertisedDevice* advDevice = nullptr;
bool deviceFound = false;

// UUIDs padrão para HID: Serviço HID "1812" e Característica de relatório "2A4D"
BLEUUID hidServiceUUID("1812");
BLEUUID reportCharUUID("2A4D");

// ===================== Configurações WebSocket =====================
WebSocketsClient webSocket;
const char* ws_server = "websocket.mirako.org";
const uint16_t ws_port = 80;
const char* ws_path = "/ws/";

// ===================== Mapeamento pré-definido (raw -> comando) =====================
struct Mapping {
  String raw;
  String command;
};

Mapping mappings[] = {
  { "00 80 00 80 00 80 00 80 00 00 00 00 01 00 00", "UP" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 05 00 00", "DOWN" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 07 00 00", "LEFT" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 03 00 00", "RIGHT" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 01 00", "A" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 02 00", "B" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 08 00", "C" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 04 00", "D" },
  { "00 80 00 80 00 80 00 80 ff 03 00 00 09 00 00", "R1" },
  { "00 80 00 80 00 80 00 80 00 00 ff 03 09 00 00", "R2" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 10 00", "L1" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 20 00", "L2" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 40 00", "Start" },
  { "00 80 00 80 00 80 00 80 00 00 00 00 09 80 00", "Select" }
};

const int mappingsCount = sizeof(mappings) / sizeof(mappings[0]);

// ===================== Debounce para envio de comandos =====================
String lastCommandSent = "";
unsigned long lastCommandTime = 0;
const unsigned long commandDebounceDelay = 50; // 500ms

// ===================== Função para converter dados raw em hexadecimal =====================
String rawDataToHex(uint8_t* data, size_t length) {
  String hexStr = "";
  for (size_t i = 0; i < length; i++) {
    if (data[i] < 0x10) hexStr += "0";
    hexStr += String(data[i], HEX);
    if (i < length - 1) hexStr += " ";
  }
  return hexStr;
}

// ===================== Função para obter comando a partir do raw =====================
String getCommandFromRaw(String rawVal) {
  for (int i = 0; i < mappingsCount; i++) {
    if (rawVal.equals(mappings[i].raw)) {
      return mappings[i].command;
    }
  }
  return "Unknown";
}

// ===================== Callbacks BLE =====================

// Callback para dispositivos encontrados durante o scan
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  public:
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("Dispositivo encontrado: ");
      Serial.println(advertisedDevice.toString().c_str());
      if (advertisedDevice.haveName() && advertisedDevice.getName().compareTo(targetName) == 0) {
        Serial.println(">> Dispositivo alvo encontrado!");
        advDevice = new BLEAdvertisedDevice(advertisedDevice);
        deviceFound = true;
        BLEDevice::getScan()->stop();
      }
    }
};

// Callback para eventos do cliente BLE
class MyClientCallbacks : public BLEClientCallbacks {
  public:
    void onConnect(BLEClient* pClient) {
      Serial.println("Cliente BLE conectado");
    }
    void onDisconnect(BLEClient* pClient) {
      Serial.println("Cliente BLE desconectado");
    }
};

// Callback para notificações BLE HID
void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  String rawVal = rawDataToHex(pData, length);
  String commandName = getCommandFromRaw(rawVal);

  // Apenas envie se o comando não for "Unknown" e aplicar debounce
  if (commandName != "Unknown" &&
      (commandName != lastCommandSent || (millis() - lastCommandTime >= commandDebounceDelay))) {
    lastCommandSent = commandName;
    lastCommandTime = millis();
    Serial.print("Comando: ");
    Serial.println(commandName);
    webSocket.sendTXT(commandName);
  }
}

// ===================== Callback WebSocket =====================
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WS] Desconectado");
      break;
    case WStype_CONNECTED:
      Serial.print("[WS] Conectado: ");
      Serial.println((char*)payload);
      webSocket.sendTXT("ESP32 conectado e pronto!");
      break;
    case WStype_TEXT:
      Serial.printf("[WS] Texto recebido: %s\n", payload);
      break;
    case WStype_BIN:
      Serial.printf("[WS] Dados binários recebidos, tamanho: %d\n", length);
      break;
    case WStype_PING:
      Serial.println("[WS] Ping recebido");
      break;
    case WStype_PONG:
      Serial.println("[WS] Pong recebido");
      break;
    default:
      break;
  }
}

// ===================== Tasks =====================

// Task para gerenciamento do WebSocket
void taskWebSocket(void* parameter) {
  webSocket.begin(ws_server, ws_port, ws_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(15000, 3000, 2);
  
  for (;;) {
    webSocket.loop();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// Task para gerenciamento do BLE
void taskBLE(void* parameter) {
  BLEDevice::init("");
  
  BLEScan* pScan = BLEDevice::getScan();
  pScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pScan->setActiveScan(true);
  pScan->setInterval(100);
  pScan->setWindow(99);
  
  Serial.println("Iniciando scan BLE...");
  pScan->start(10, false);
  
  if (!deviceFound) {
    Serial.println("Dispositivo alvo não encontrado!");
    vTaskDelete(NULL);
    return;
  }
  
  Serial.print("Tentando conectar ao dispositivo: ");
  Serial.println(advDevice->getAddress().toString().c_str());
  
  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallbacks());
  
  unsigned long startTime = millis();
  bool connected = false;
  const unsigned long timeout = 10000; // 10 segundos
  while (!connected && (millis() - startTime < timeout)) {
    connected = pClient->connect(advDevice);
    if (!connected) {
      Serial.println("Tentando reconectar...");
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
  
  if (!connected) {
    Serial.println("Falha na conexão com o controle: Timeout");
    vTaskDelete(NULL);
    return;
  }
  Serial.println("Conectado com sucesso ao controle!");
  
  BLERemoteService* pRemoteService = pClient->getService(hidServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.println("Falha ao encontrar o serviço HID");
    pClient->disconnect();
    vTaskDelete(NULL);
    return;
  }
  Serial.println("Serviço HID encontrado");
  
  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(reportCharUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("Falha ao encontrar a característica de relatório");
    pClient->disconnect();
    vTaskDelete(NULL);
    return;
  }
  Serial.println("Característica de relatório encontrada");
  
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
    Serial.println("Callback de notificação registrado");
  } else {
    Serial.println("A característica não suporta notificações");
  }
  
  // Mantém a task ativa para receber notificações
  for (;;) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ===================== Setup e Loop =====================
void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando ESP32...");
  
  // Conexão WiFi
  Serial.println("Conectando ao WiFi...");
  wifiMulti.addAP(ssid, password);
  while (wifiMulti.run() != WL_CONNECTED) {
    vTaskDelay(500 / portTICK_PERIOD_MS);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado ao WiFi, IP: ");
  Serial.println(WiFi.localIP());
  
  // Cria task para o WebSocket
  xTaskCreatePinnedToCore(taskWebSocket, "WebSocketTask", 8192, NULL, 1, NULL, 0);
  // Cria task para o BLE
  xTaskCreatePinnedToCore(taskBLE, "BLETask", 8192, NULL, 1, NULL, 1);
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
