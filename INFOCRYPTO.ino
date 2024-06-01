
 #if (TELA == 1)




struct CryptoDetail {
  String name;
  String url;
};

// Lista de criptomoedas/dólar para buscar
CryptoDetail cryptoDetails[] = {
  {"DOLAR", "https://economia.awesomeapi.com.br/json/last/USD"},
 // {"BTC", "http://api.coindesk.com/v1/bpi/currentprice/BTC.json"},
  {"BTC",   "https://www.bitstamp.net/api/v2/ticker/btcusd"},
  {"DOGE",  "https://www.bitstamp.net/api/v2/ticker/dogeusd"},
  {"XMR",   "https://api.bitfinex.com/v1/pubticker/xmrusd"},
  {"SHIBA", "https://www.bitstamp.net/api/v2/ticker/shibusd"},
  {"LTC",   "https://www.bitstamp.net/api/v2/ticker/ltcusd"},
  {"ETH",   "https://www.bitstamp.net/api/v2/ticker/ethusd"},
  {"LINK",  "https://www.bitstamp.net/api/v2/ticker/linkusd"},
  {"RNDR",  "https://www.bitstamp.net/api/v2/ticker/rndrusd"}
};



void bolsa() {
   Serial.println("EXECUTANDO BOLSA...");
    tft.setRotation(rotate);

  tft.fillScreen(TFT_BLACK); // Limpar a tela
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(0, 10);
  tft.println("PEGANDO COTACAO...");

  // String para armazenar todos os valores a serem exibidos
  String displayValues = "";

  for (int i = 0; i < sizeof(cryptoDetails) / sizeof(cryptoDetails[0]); i++) {
      String price = getCurrencyPrice(cryptoDetails[i].url);

      tft.println(cryptoDetails[i].name + ":" + price);

    if (tft.getTouchRawZ() > 500) {
      Serial.println("Tela tocada. Saindo do loop.CRYPTO");
      functionExecuted = false;  // Marca a função como executada
      break;
    }

    displayValues += cryptoDetails[i].name + ":" + price + "\n"; // Adiciona valor à string
  }

  // Exibir todos os valores de uma vez na tela
   tft.fillScreen(TFT_BLACK); // Limpar a tela
     tft.setTextColor(TFT_GREEN);
     tft.setTextSize(4);
  tft.setCursor(0, 10);
  tft.println(displayValues);
 if (tft.getTouchRawZ() > 500) {
      Serial.println("Tela tocada. Saindo do loop.stopFetching");
      functionExecuted = false;  // Marca a função como executada
      touchCount++;
      delay(800);
    }

}

  




String getCurrencyPrice(String url) {


    
  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.println("Failed to get HTTP response.");
    return "ERRO INTERNET";
  }

  String response = http.getString();
  http.end();


  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, response);
  if (error) {
    Serial.print(F("Failed to parse JSON: "));
    Serial.println(error.c_str());
    return "ERRO parse JSON";
  }

  if (doc.containsKey("last")) {
    return doc["last"].as<String>();
  } else if (doc.containsKey("bpi")) {
    return doc["bpi"]["USD"]["rate_float"].as<String>();
  } else if (doc.containsKey("USDBRL")) {
    return doc["USDBRL"]["bid"].as<String>();
  } else if (doc.containsKey("last_price")) {
    return doc["last_price"].as<String>();
  } 

  return "N/A";
}


#endif
