#include <WiFi.h>
#include <HTTPClient.h>

// API endpoint
const char* serverName = "http://52.54.32.237/granagrow/ph/update_data.php";

// Função para enviar os dados ao servidor
void sendDataToServer() {
  if (WiFi.status() == WL_CONNECTED) { // Verifica a conexão WiFi

    HTTPClient http;

    // Obtenha o endereço MAC
    String macAddress = WiFi.macAddress();

    // Define o endpoint da API
    http.begin(serverName);

    // Especifica o tipo de conteúdo (JSON)
    http.addHeader("Content-Type", "application/json");

    // Cria um objeto JSON para enviar os dados
    String jsonData = "{\"phAjustado\": " + String(adjusted_ph_level) + 
                      ", \"phBruto\": " + String(phBruto) + 
                      ", \"temperature\": " + String(temperature) + 
                      ", \"humidity\": " + String(humidity) + 
                      ", \"pressure\": " + String(pressure) +
                      ", \"macAddress\": \"" + macAddress + "\"}";

    // Envia a solicitação POST
    int httpResponseCode = http.POST(jsonData);

    // Verifica a resposta do servidor
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
    Serial.println("WiFi desconectado");
  }
}
