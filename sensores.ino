





////#############################################################################Le sensores

float sensorValue_7 = 2048; // Valor do sensor para pH 7.0
float sensorValue_low = 1024; // Valor do sensor para pH baixo (por exemplo, 4.0)
float sensorValue_high = 3072; // Valor do sensor para pH alto (por exemplo, 10.0)


float ph() {

  int sensorValue = analogRead(phpin);
  
  // Mapeamento linear para a faixa de pH
  float pHValue = map(sensorValue, sensorValue_low, sensorValue_high, 4.0, 10.0); // Mapeia o valor do sensor para a faixa de pH (4.0 a 10.0)
  
  return pHValue;

}







//############################################CONFIG SENSOR TEMPERATURA PRESSAO
#if (SENSORES == 1)









String readDHTTemperature() {
  sensors_event_t temp;
  aht_temp->getEvent(&temp);
  
  float t = temp.temperature;

  if (isnan(t)) {    
    Serial.println("Failed to read from TEMPERATURA sensor!");
    return "00";
  }
  else {
  //  Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
sensors_event_t humidity;
  aht_humidity->getEvent(&humidity);

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = humidity.relative_humidity;
  if (isnan(h)) {
    Serial.println("Failed to read from UMIDADE sensor!");
    return "00";
  }
  else {
   // Serial.println(h);
    return String(h);
  }
}

String readDHTPressao() {
sensors_event_t pressure_event;
  bmp_pressure->getEvent(&pressure_event);

  float p = pressure_event.pressure;
  if (isnan(p)) {
    Serial.println("Failed to read sensor!");
    return "00";
  }
  else {
    return String(p);
  }
}














#endif
