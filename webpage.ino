


void startAPMode() {
    // Iniciar o modo AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP("GrANAConfig"); // Nome da rede AP
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("End IP do AP: ");
    Serial.println(myIP);

    // Inicializa o servidor web
    server.on("/", handleRoot);
    server.on("/scan", HTTP_GET, handleScan);
    server.on("/connect", handleConnect);
    server.begin();
    Serial.println("Servidor AP iniciado");
}

void startWebServer() {
    // Configura o certificado CA
 //   client.setCACert(caCert);
    // WiFi.config(IPAddress(), dns1, dns2);

    setClock();

    // Inicializa o servidor web
    server.on("/", handleMain);
    server.on("/salvar", handleSalvar);
    server.on("/deletewifi", deletewififile);
    server.begin();
    Serial.println("Servidor web iniciado");
    
    Serial.print("Servidor DNS 1:");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("Servidor DNS 2:");
    Serial.println(WiFi.dnsIP(1));
    Serial.print("LocalIP:");
    Serial.println(WiFi.localIP().toString());

    
    
}



//#########################################Cria paginas WEB
void handleSalvar() {
    String nomedobot = server.arg("nomedobot");
  String usuario = server.arg("usuario");

    saveWifiCredentials(ssid.c_str(), password.c_str(), nomedobot.c_str(), geo.c_str(), usuario.c_str());

        server.send(200, "text/plain", "SALVO!  ");

  }
void handleMain() {



  
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; }";
  html += "h1 { color: #333; }";
  html += "h2 { color: #666; }";
  html += "p { margin: 10px 0; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>GRANANET WEBPAGE</h1>";
  html += "<h2>NOMEBOT:</h2>";
  html += "<p class='data'>" + nomedobot + "</p>";
  html += "<h2>GEO:</h2>";
  html += "<p class='data'>" + geo + "</p>";
  html += "<h2>SSID:</h2>";
  html += "<p class='data'>" + ssid + "</p>";
  html += "<h2>MENSAGEM:</h2>";
  html += "<p class='data'>" + String(mensagem) + "</p>";
  html += "<h2>ESTADO:</h2>";
  html += "<p class='data'>" + String(estado) + "</p>";
  html += "<h2>USUARIO:</h2>";
  html += "<p class='data'>" + String(usuario) + "</p>";
  html += "<form method='post' action='/salvar'>";
  html += "<label for='usuario'>Usuario:</label><input type='text' name='usuario'><br>"; // Novo input
  html += "<label for='nomedobot'>Nome do BOT:</label><input type='text' name='nomedobot'><br>";
  html += "<input type='submit' value='SALVAR'>";
  html += "</form>";
  //html += "<form action='/deletewifi' method='post'><button type='submit'>Delet Config WiFi </button></form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleRoot() {
  loadWifiCredentials(ssid, password, nomedobot, geo, usuario); // Carrega os dados

  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; }";
  html += "h1 { color: #333; }";
  html += "h2 { color: #666; }";
  html += "p { margin: 10px 0; }";
  html += ".data { font-weight: bold; }";
  html += ".button { background-color: #007bff; color: #fff; padding: 10px 20px; border: none; cursor: pointer; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>CONFIG GRANANET</h1>";
  html += "<h2>NOMEBOT:</h2>";
  html += "<p class='data'>" + nomedobot + "</p>";
  html += "<h2>GEO:</h2>";
  html += "<p class='data'>" + geo + "</p>";
  html += "<h2>SSID:</h2>";
  html += "<p class='data'>" + ssid + "</p>";
  html += "<h2>password:</h2>";
  html += "<p class='data'>" + password + "</p>";
  html += "<h2>Usuario:</h2>"; 
  html += "<p class='data'>" + usuario + "</p>";
  html += "<form method='get' action='/scan'>";
  html += "<input type='submit' class='button' value='Escanear Redes'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleScan() {
  Serial.println("SCAN");
  // Send an HTTP response
  String html = "<html><head><style>";
  html += "body { font-family: Arial, Helvetica, sans-serif; background-color: #f4f4f4; text-align: center; }";
  html += "h1 { color: #333; }";
  html += "ul { list-style-type: none; padding: 0; }";
  html += "li { margin: 10px 0; }";
  html += "label { font-weight: bold; }";
  html += "input[type='text'], input[type='text'] { width: 100%; padding: 10px; margin: 5px 0; }";
  html += "input[type='submit'], input[type='button'] { background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer; }";
  html += "a { text-decoration: none; }";
  html += "</style></head><body>";
  html += "<h1>Selecione Wi-Fi Network:</h1>";
  html += "<form method='post' action='/connect'>";
  html += "<ul>";
  WiFi.disconnect();

  // Scan for Wi-Fi networks
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    html += "<h1>No Wi-Fi Network:</h1>";
    Serial.println("no networks found");
  } else {
    for (int i = 0; i < n; i++) {
      // Add each network to the HTML list
      String ssid = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);
      html += "<li><input type='radio' name='ssid' value='" + ssid + "'>" + ssid + " (RSSI: " + String(rssi) + ")</li>";
    }
  }

  html += "</ul>";
  html += "<label for='password'>Password:</label><input type='text' name='password'><br>";
  html += "<label for='nomedobot'>Nome do BOT:</label><input type='text' name='nomedobot'><br>";
  html += "<label for='geo'>Geo Localizacao:</label><input type='text' name='geo'><br>";
  html += "<label for='usuario'>Usuario:</label><input type='text' name='usuario'><br>"; // Novo input
  html += "<input type='submit' value='CONECTAR'>";
  html += "</form>";
  html += "<p><br><br><br><a href='/'>Back</a></p>";
  html += "<input type='button' value='PESQUISAR NOVAMENTE' onclick='location.reload()'>";
  html += "</body></html>";

  // Send the HTML response
  server.send(200, "text/html", html);
}


void handleConnect() {
  Serial.println("CONECT");

  String ssid = server.arg("ssid");
  String password = server.arg("password");
  String nomedobot = server.arg("nomedobot");
  String geo = server.arg("geo");
  String usuario = server.arg("usuario");

  saveWifiCredentials(ssid.c_str(), password.c_str(), nomedobot.c_str(), geo.c_str(), usuario.c_str());

  // Verifique se o SSID e a senha NAO ESTA vazios
  if (ssid.length() > 0 && password.length() > 0) {
    WiFi.begin(ssid.c_str(), password.c_str());

    // Aguarde a conexxao ser estabelecida
    int timeout = 10; // Tempo limite para a conex��o em segundos
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
      delay(1000); // Aguarde 1 segundo
      timeout--;
    }

    if (WiFi.status() == WL_CONNECTED) {
      String configg = "Prontinho. GRANANET reiniciando, conecte no seu WIFI e clique no endereco abaixo IP: " + WiFi.localIP().toString();
      configg += "<br><br>";
      configg += "CLIQUE -> VVVVVVVV";
      configg += "<br><br>";
      configg += "<a href='http://";
      configg += WiFi.localIP().toString();
      configg += "'>";
      configg += nomedobot.c_str();
      configg += "</a>";
      server.send(200, "text/html", configg);
      delay(5000);
      ESP.restart();
    } else {
      server.send(200, "text/plain", "n conectado!  ");
    }
  }
}
