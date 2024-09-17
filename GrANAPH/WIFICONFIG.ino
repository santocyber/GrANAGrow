
// Função para conectar-se ao Wi-Fi
void connectToWiFi() {
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA); // Garantir que estamos em modo STA (cliente)
  WiFi.begin(ssid, password);
  WiFi.setSleep(false); // Desativar o modo de economia de energia, caso esteja ativado.
  WiFi.setHostname("GrANAGrowPH"); 
  Serial.println("Tentando conectar ao Wi-Fi...");
  Serial.println("SSID: " + String(ssid));
  Serial.println("Password: " + String(password));

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.println("Tentando conectar ao Wi-Fi...");

  int max_attempts = 20;
  while (WiFi.status() != WL_CONNECTED && max_attempts-- > 0) {
    delay(500);
    Serial.print(".");
    
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print(".");

  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("Wi-Fi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(4);
  tft.setCursor(0, 100);
  tft.println("Wi-Fi conectado!");
  tft.println("Endereco IP: ");
  tft.println(WiFi.localIP());
  delay(1000); // Pequeno atraso para estabilizar

  
  } else {
    Serial.println("");
    Serial.println("Falha ao conectar-se ao Wi-Fi.");
  }
}
void startAccessPoint() {
  WiFi.disconnect(true); // Desconectar qualquer conexão Wi-Fi anterior
  delay(1000); // Pequeno atraso para estabilizar
  WiFi.mode(WIFI_AP_STA); // Modo AP (ponto de acesso)
  delay(500); // Atraso para garantir a estabilidade do AP

  WiFi.softAP("GrANAGrowAP", "12345678"); // Definir senha do AP como '12345678'

  Serial.println("Access Point iniciado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.softAPIP());

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(4);
  tft.setCursor(0, 100);
  tft.println("Access Point");
  tft.println("Endereco IP: ");
  tft.println(WiFi.softAPIP());
  delay(1000); // Pequeno atraso para estabilizar

  apActive = true;

  server.on("/", []() {
    String html = "<html><head><style>";
    html += "body { font-family: Arial, sans-serif; background-color: #121212; color: #e0e0e0; margin: 0; padding: 20px; }";
    html += "h2 { color: #bb86fc; }";
    html += "button { background-color: #bb86fc; color: #121212; padding: 10px 20px; border: none; cursor: pointer; margin-bottom: 20px; }";
    html += "table { width: 100%; border-collapse: collapse; }";
    html += "th, td { text-align: left; padding: 10px; }";
    html += "th { background-color: #1f1f1f; color: #bb86fc; }";
    html += "tr:nth-child(even) { background-color: #2a2a2a; }";
    html += "tr:hover { background-color: #383838; }";
    html += "input[type='radio'] { margin-right: 10px; }";
    html += "</style></head><body>";
    html += "<h2>Config Wi-Fi GrANAGrow</h2>";
    html += "<button onclick='scanNetworks()'>Escanear Redes</button>";
    html += "<form action='/save' method='post'>";
    html += "<table id='wifiTable'><thead><tr><th>Selecionar</th><th>SSID</th><th>Sinal (dBm)</th></tr></thead><tbody></tbody></table><br>";
    html += "Password: <input type='text' name='password' required><br><br>";
    html += "<input type='submit' value='Salvar'>";
    html += "</form>";
    html += "<button onclick='clearCredentials()'>Limpar Credenciais Wi-Fi</button>";
    html += "<script>";
    html += "function scanNetworks() {";
    html += "fetch('/scan').then(response => response.json()).then(data => {";
    html += "let tableBody = document.getElementById('wifiTable').getElementsByTagName('tbody')[0];";
    html += "tableBody.innerHTML = '';";
    html += "data.forEach(network => {";
    html += "let row = tableBody.insertRow();";
    html += "let selectCell = row.insertCell(0);";
    html += "let ssidCell = row.insertCell(1);";
    html += "let signalCell = row.insertCell(2);";
    html += "selectCell.innerHTML = `<input type='radio' name='ssid' value='${network.ssid}' required>`;";
    html += "ssidCell.textContent = network.ssid;";
    html += "signalCell.textContent = network.signal;";
    html += "});";
    html += "});";
    html += "}";
    html += "function clearCredentials() {";
    html += "fetch('/clear').then(() => {";
    html += "alert('Credenciais Wi-Fi limpas. Por favor, configure novamente.');";
    html += "location.reload();";
    html += "});";
    html += "}";
    html += "</script></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/scan", []() {
    int n = WiFi.scanNetworks();
    String json = "[";
    for (int i = 0; i < n; i++) {
      if (i > 0) json += ",";
      int signal = WiFi.RSSI(i);
      json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"signal\":" + String(signal) + "}";
    }
    json += "]";
    server.send(200, "application/json", json);
  });

  server.on("/save", []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    saveWiFiCredentials(ssid.c_str(), password.c_str());
    server.send(200, "text/html", "<html><body><h2>Credenciais salvas. Reiniciando o dispositivo.</h2></body></html>");
    delay(1000);
    ESP.restart();
  });

  server.on("/clear", []() {
    FFat.remove("/wifi.txt"); // Remove o arquivo contendo as credenciais Wi-Fi
    server.send(200, "text/html", "<html><body><h2>Credenciais Wi-Fi foram limpas.</h2></body></html>");
    delay(1000);
    ESP.restart(); // Reinicia o ESP32 após limpar as credenciais
  });

  server.begin();
}


// Função para iniciar o FATFS
void startFATFS() {
  if (!FFat.begin(true)) {
    Serial.println("Erro ao montar o sistema de arquivos!");
    return;
  }
}

// Função para carregar as credenciais do Wi-Fi do arquivo
bool loadWiFiCredentials() {
  File file = FFat.open("/wifi.txt", "r");
  if (!file) {
    Serial.println("Credenciais de Wi-Fi não encontradas.");
    return false;
  }

  String ssidStr = file.readStringUntil('\n');
  String passStr = file.readStringUntil('\n');
  ssidStr.trim();
  passStr.trim();
  ssidStr.toCharArray(ssid, 32);
  passStr.toCharArray(password, 64);
  
  file.close();

  return true;
}

// Função para salvar as credenciais de Wi-Fi no arquivo
void saveWiFiCredentials(const char* ssid, const char* password) {
  File file = FFat.open("/wifi.txt", "w");
  if (file) {
    file.println(ssid);
    file.println(password);
    file.close();
    Serial.println("Credenciais de Wi-Fi salvas com sucesso.");
  } else {
    Serial.println("Falha ao salvar credenciais de Wi-Fi.");
  }
}
