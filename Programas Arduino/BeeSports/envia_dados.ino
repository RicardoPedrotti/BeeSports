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

String criaJson(String serial_dispositivo, int bpm, int passos) {
  String Json = "";
  Json = "{\n\"CD_SERIAL_DISPOSITIVO\": \"" + serial_dispositivo + "\",\n\"QTD_BATIMENTOS_CARDIACOS\": " + bpm + ",\n\"QTD_PASSOS\": " + passos + "\n}";
  //Serial.println(Json);
  return Json;
}

