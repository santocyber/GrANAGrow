

//#########################SALVA INFO WIFI NO SPIFF

void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo, const String &usuario) {
  File file = SPIFFS.open("/wifi_credentials.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo para salvar as credenciais.");
    return;
  }

  file.println(ssid);
  file.println(password);
  file.println(nomedobot);
  file.println(geo);
  file.println(usuario);

  file.close();
}



void  loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo, String &usuario) {
  File file = SPIFFS.open("/wifi_credentials.txt", "r");
  if (!file) {
    Serial.println("Arquivo de credenciais n         o encontrado.");
    return;
  }

  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  nomedobot = file.readStringUntil('\n');
  geo = file.readStringUntil('\n');
  usuario = file.readStringUntil('\n');

  file.close();
}

  void deletewififile() {
String ssid0 ="";
String password0 ="";
String nomedobot0 ="";
String geo0 ="";
String usuario0 ="";
        
          saveWifiCredentials(ssid0,password0,nomedobot0,geo0,usuario0);

          server.send(200, "text/plain", "WiFi file deleted");

          ESP.restart();

}










// Fun                  o para carregar o SSID e a senha do SPIFFS




//################################ Fun                  o para salvar no arquivo txt



void savefile(const String &estado, const String &mensagem, const String &mensagemstatus) {
  File file = SPIFFS.open("/estado.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo.");
    return;
  }

  file.println(estado);
  file.println(mensagem);
  file.println(mensagemstatus);
file.close();
}

void saveTime(const String &hrliga, const String &hrdesliga, const String &timerautomatico, const String &timerfoto, const String &timerfotostatus) {
  File file = SPIFFS.open("/horarios.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo.");
    return;
  }

  file.println(hrliga);
  file.println(hrdesliga);
  file.println(timerautomatico);
  file.println(timerfoto);
  file.println(timerfotostatus);
  file.close();
}

void loadfile(String &estado, String &mensagem, String &mensagemstatus) {
  File file = SPIFFS.open("/estado.txt", "r");
  if (!file) {
    Serial.println("Arquivo nao encontrado.");
    return;
  }

  estado = file.readStringUntil('\n');
  mensagem = file.readStringUntil('\n');
  mensagemstatus = file.readStringUntil('\n');
  
  file.close();
}

void loadTime(String &hrliga, String &hrdesliga, String &timerautomatico, String &timerfoto, String &timerfotostatus) {
  File file = SPIFFS.open("/horarios.txt", "r");
  if (!file) {
    Serial.println("Arquivo nao encontrado.");
    return;
  }

  hrliga = file.readStringUntil('\n');
  hrdesliga = file.readStringUntil('\n');
  timerautomatico = file.readStringUntil('\n');
  timerfoto = file.readStringUntil('\n');
  timerfotostatus = file.readStringUntil('\n');

  file.close();
}

void deletefile() {
String estado0 ="";
String mensagem0 ="";
String mensagemstatus0 ="";
String hrliga0 ="";
String hrdesliga0 ="";
String timerautomatico0 ="";
String timerfoto0 ="";
String timerfotostatus0 ="";

savefile(estado0, mensagem0, mensagemstatus0);
saveTime(hrliga0, hrdesliga0, timerautomatico0, timerfoto0, timerfotostatus0);
   



        
}
