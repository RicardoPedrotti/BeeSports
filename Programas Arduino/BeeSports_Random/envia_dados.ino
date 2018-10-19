/*
  JsonObject criaJson(String serial_dispositivo, int bpm, int passos){
  const size_t bufferSize = JSON_OBJECT_SIZE(3);
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.createObject();
  root["CD_SERIAL_DISPOSITIVO"] = serial_dispositivo;
  root["QTD_BATIMENTOS_CARDIACOS"] = bpm;
  root["QTD_PASSOS"] = passos;

  root.printTo(Serial);
  return root;
  }
*/

String criaJson(String serial_dispositivo, int bpm, String horario, int passos) {
  String Json = "";
  Json = "{\n\"CD_DISP\": \"" + serial_dispositivo
  + "\",\n\"QT_BPM\": " + bpm
  + ",\n\"HORA\": \"" + horario
  + "\",\n\"QT_PASS\": " + passos + "\n}";
  //Serial.println(Json);
  return Json;
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

