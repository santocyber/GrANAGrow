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
