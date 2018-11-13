#include<Arduino.h>
#include<Wire.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int ledvermelho = 14; //D5
int ledverde = 12; //D6

//Endereco I2C do MPU6050
const uint8_t MPU = 0x68;

String serial = "BEE0001";

//unsigned long millisinicio;
//unsigned long millisatual;
//const unsigned long periodo = 1000; //1 segundo em millis

const char* mqtt_server = "54.94.143.174";

WiFiClient espClient;
PubSubClient client(espClient);

//###############################  ARDUINO  ########################################
void setup()
{
  Serial.println("################  Inicia Setup  ################");
  iniciaSerial();
  delay(500);
  ligaLed(ledvermelho, ledverde);
  delay(500);
  iniciaWifi();
  delay(500);
  piscaLed(ledvermelho, 500, 1);
  delay(500);
  iniciaTempo();
  delay(500);
  piscaLed(ledvermelho, 500, 1);
  delay(500);
  Wire.begin();
  delay(500);
  piscaLed(ledvermelho, 500, 1);
  delay(200);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(500);
  piscaLeds(ledvermelho, ledverde, 50, 3);
  //millisinicio = millis();
  Serial.println("################  Fim do Setup  ################");
}
void loop()
{
  String cd_serial_dispo = "";
  int bpm = 0;
  String horario = "";
  int passos = 0;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  cd_serial_dispo = serial;
  bpm = random(70, 180);
  horario = calculaHorario();
  passos = random(1, 10);

  String mqttmsg = criaJson(cd_serial_dispo, bpm, horario, passos);
  int str_len = mqttmsg.length() + 3;
  char msg[str_len];

//  mqttmsg = "\"" + mqttmsg + "\"";

  mqttmsg.toCharArray(msg, str_len);
    
  
  //int value = 0;
  //snprintf (msg, 75, mqttmsg, value);
  Serial.print("Publish message: ");
  Serial.println("Hello World");
  Serial.println(msg);

  bool x = client.publish("outTopic", msg);// BEE0001, QTD_BATIMENTOS_CARDIACOS: 96, HORARIO_MEDICAO: 17/10/2018 19:26:50, TD_PASSOS: 8 }");
  piscaLed(ledverde, 200, 1);
  if (x){
    Serial.println("DEVIA TER IDO E SE N FOI VCS SAO UNS NOOB");
  }else{
    Serial.println("ESCAFEDEU-SE");
  }

  delay(3000);
}
