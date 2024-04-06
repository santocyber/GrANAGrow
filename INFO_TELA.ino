




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
   unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 80000) {

    
  }
    tft.fillScreen(TFT_BLACK); // Limpar a tela

    // Exibir os valores das strings no display
    tft.setTextColor(TFT_YELLOW);
    tft.setTextSize(4);
    
    // Exibir os valores das criptomoedas/dólar no display
    int yOffset = 10;
    for (int i = 0; i < sizeof(cryptoDetails) / sizeof(cryptoDetails[0]); i++) {


            
  if (tft.getTouchRawZ() > 500) {
                Serial.println("Tela tocada. Saindo do loop.stopFetching");
                functionExecuted = false;  // Marca a função como executada

                break; 
              
            }

            
      String price = getCurrencyPrice(cryptoDetails[i].url);
      tft.setCursor(10, yOffset);
      tft.println(cryptoDetails[i].name + ":" + price);
      yOffset += 30;



    

  }

  // atualizar o valor de previousMillis para o momento atual
    previousMillis = currentMillis;
  }

  






String getCurrencyPrice(String url) {
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.println("Failed to get HTTP response");
    return "N/A";
  }

  String response = http.getString();
  http.end();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, response);

  if (error) {
    Serial.print(F("Failed to parse JSON: "));
    Serial.println(error.c_str());
    return "N/A";
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
