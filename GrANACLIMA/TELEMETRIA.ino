#include <WiFi.h>
#include <HTTPClient.h>

// API endpoint
const char* serverName = "http://52.54.32.237/granagrow/clima/update_data.php";

// Função para enviar dados ao servidor
void sendDataToServer(float temperature, float humidity, float pressure) {
  if (WiFi.status() == WL_CONNECTED) { // Verifica o status da conexão Wi-Fi

    HTTPClient http;

    // Obtenha o endereço MAC
    String macAddress = WiFi.macAddress();

    // Defina o endpoint da API
    http.begin(serverName);

    // Especifique o tipo de conteúdo (JSON)
    http.addHeader("Content-Type", "application/json");

    // Crie um objeto JSON para enviar os dados
    String jsonData = "{\"temperature\": " + String(temperature) + 
                      ", \"humidity\": " + String(humidity) + 
                      ", \"pressure\": " + String(pressure) + 
                      ", \"macAddress\": \"" + macAddress + "\"}";

    // Envie a solicitação POST
    int httpResponseCode = http.POST(jsonData);

    // Verifique a resposta do servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Código de resposta HTTP: " + String(httpResponseCode));
      Serial.println("Resposta do servidor: " + response);
    } else {
      Serial.println("Erro ao enviar POST: " + String(httpResponseCode));
    }

    // Fecha a conexão
    http.end();
  } else {
    Serial.println("WiFi Desconectado");
  }
}
