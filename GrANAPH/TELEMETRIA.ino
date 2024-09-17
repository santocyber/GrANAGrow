#include <WiFi.h>
#include <HTTPClient.h>


// API endpoint
const char* serverName = "http://52.54.32.237/granagrow/ph/update_data.php";



void sendDataToServer() {
  if (WiFi.status() == WL_CONNECTED) { // Check WiFi connection status

    HTTPClient http;

    // Obtenha o endereço MAC
    String macAddress = WiFi.macAddress();

    // Defina o endpoint da API
    http.begin(serverName);

    // Especifique o tipo de conteúdo (JSON)
    http.addHeader("Content-Type", "application/json");

    // Crie um objeto JSON para enviar os dados
    String jsonData = "{\"phValue\": " + String(ph4502c.read_ph_level()) + ", \"temperature\": " + String(temperature) + 
                      ", \"humidity\": " + String(humidity) + ", \"pressure\": " + String(pressure) +
                      ", \"macAddress\": \"" + macAddress + "\"}";

    // Envie a solicitação POST
    int httpResponseCode = http.POST(jsonData);

    // Verifique a resposta do servidor
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));
    }

    // Fecha a conexão
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
