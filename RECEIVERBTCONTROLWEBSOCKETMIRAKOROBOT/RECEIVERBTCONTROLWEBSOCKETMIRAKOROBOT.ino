#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebSocketsClient_Generic.h>

WiFiMulti wifiMulti;
const char* ssid = "InternetSA";
const char* password = "cadebabaca";

// Configurações do WebSocket
WebSocketsClient webSocket;
const char* ws_server = "websocket.mirako.org";  // sem o ws:// se a biblioteca já tratar isso
const uint16_t ws_port = 80;
const char* ws_path = "/ws/";

// Definições de comandos conforme o código do Arduino UNO
#define CMD_FORWARD       92   // "UP" -> mover para frente
#define CMD_BACKWARD      163  // "DOWN" -> mover para trás
#define CMD_MOTORLEFT     230  // "LEFT" -> motor esquerda
#define CMD_MOTORRIGHT    231  // "RIGHT" -> motor direita
#define CMD_CLOCKWISE     83   // "R1" -> rotação horária
#define CMD_CONTRAROTATE  172  // "R2" -> rotação anti-horária
#define CMD_MODEL1        25   // "A" -> selecionar modelo 1
#define CMD_MODEL2        26   // "B" -> selecionar modelo 2
#define CMD_MODEL3        27   // "C" -> selecionar modelo 3
#define CMD_MODEL4        28   // "D" -> selecionar modelo 4

// Mapeamento: comando recebido (string) -> valor de comando
int getCommandValue(String cmd) {
  cmd.trim();
  if(cmd == "UP")         return CMD_FORWARD;
  else if(cmd == "DOWN")  return CMD_BACKWARD;
  else if(cmd == "LEFT")  return CMD_MOTORLEFT;
  else if(cmd == "RIGHT") return CMD_MOTORRIGHT;
  else if(cmd == "R1")    return CMD_CLOCKWISE;
  else if(cmd == "R2")    return CMD_CONTRAROTATE;
  else if(cmd == "A")     return CMD_MODEL1;
  else if(cmd == "B")     return CMD_MODEL2;
  else if(cmd == "C")     return CMD_MODEL3;
  else if(cmd == "D")     return CMD_MODEL4;
  else return -1;  // comando desconhecido
}

// Debounce para evitar envios repetidos
String lastCommandSent = "";
unsigned long lastCommandTime = 0;
const unsigned long commandDebounceDelay = 200; // 200ms

// Envia um pacote para o Arduino via Serial2.
// O pacote tem o formato: [0xA5, comando, 0x5A]
void sendCommandToArduino(int command) {
  byte packet[3];
  packet[0] = 0xA5;
  packet[1] = (byte) command;
  packet[2] = 0x5A;
  Serial2.write(packet, 3);
  Serial.print("Enviado para Arduino: 0xA5 ");
  Serial.print(command, HEX);
  Serial.println(" 0x5A");
}

// Callback de eventos do WebSocket
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("[WS] Desconectado");
      break;
    case WStype_CONNECTED:
      Serial.print("[WS] Conectado: ");
      Serial.println((char*)payload);
      webSocket.sendTXT("ESP32 gateway conectado!");
      break;
    case WStype_TEXT:
      {
        String message = String((char*)payload);
        Serial.print("Mensagem recebida: ");
        Serial.println(message);
        int cmdValue = getCommandValue(message);
        if(cmdValue != -1 && (message != lastCommandSent || (millis() - lastCommandTime > commandDebounceDelay))) {
          lastCommandSent = message;
          lastCommandTime = millis();
          sendCommandToArduino(cmdValue);
        }
        else if(cmdValue == -1) {
          Serial.println("Comando desconhecido: " + message);
        }
      }
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

void setup() {
  Serial.begin(115200);
  // Inicialize Serial2 para comunicação com o Arduino. 
  // Ajuste os pinos conforme necessário (aqui usamos RX=16, TX=17).
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  
  Serial.println("Inicializando ESP32 Gateway...");
  
  wifiMulti.addAP(ssid, password);
  Serial.print("Conectando ao WiFi");
  while(wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectado, IP: ");
  Serial.println(WiFi.localIP());
  
  // Configuração do WebSocket
  webSocket.begin(ws_server, ws_port, ws_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(15000, 3000, 2);

    setupCAM();

}

void loop() {
  webSocket.loop();
  delay(10);
}
