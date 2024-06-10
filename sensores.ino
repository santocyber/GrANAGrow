


//#######################SENSOR PIR

#if (PIR == 1)
// Variável para contar o número de movimentos detectados
int movimentoContagem = 0;

// Variável para armazenar o tempo da última detecção de movimento
unsigned long ultimoMovimentoMillis = 0;

// Tempo de espera em milissegundos
const unsigned long esperaMovimento = 60000; // 10 segundos

void pir() {
  // Verificar se o sensor PIR detectou movimento
  if (digitalRead(PIR_PIN) == HIGH) {
    Serial.println("Movimento detectado!");
    movimentoContagem++;
    digitalWrite(PIR_PIN, LOW);

    if (movimentoContagem = 3) {
      ultimoMovimentoMillis = millis(); // Atualizar o tempo da última detecção de movimento
      Serial.println(movimentoContagem);
      Serial.println("Movimentos detectados! Acionando a captura.");
      sendPhotoToAPI();
      capturaimagemenviabd();

    }
  }
  
    // Verificar se já se passaram 10 segundos desde a última detecção de movimento
    if (millis() - ultimoMovimentoMillis >= esperaMovimento) {
      // Resetar a contagem se não houver movimento por 30 segundos
      movimentoContagem = 0;

    }
  

  delay(100); // Pequeno atraso para evitar leitura excessiva do PIR
}

#endif

#if (PH == 1)

float ph() {
float phshow;

   sensorValue = analogRead(phpin);

  
  // Mapeamento linear para a faixa de pH
  float pHValue = sensorValue*(4.3/4095.0);
    phshow=(3.3*pHValue);


  
      Serial.println(sensorValue);
    Serial.println(phshow);




  return phshow;

}


#endif




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
