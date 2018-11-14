#include<Arduino.h>
#include<Wire.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <math.h>

int ledvermelho = 14; //D5
int ledverde = 12; //D6

WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "54.94.143.174";

//Endereco I2C do MPU6050
const uint8_t MPU = 0x68;

//unsigned long millisinicio;
//unsigned long millisatual;
//const unsigned long periodo = 1000; //1 segundo em millis

const int sda_pin = D2; // definição do pino I2C SDA
const int scl_pin = D1;

struct rawdata {
  int16_t AcX;
  int16_t AcY;
  int16_t AcZ;
  int16_t Tmp;
  int16_t GyX;
  int16_t GyY;
  int16_t GyZ;
};
struct scaleddata {
  float AcX;
  float AcY;
  float AcZ;
  float Tmp;
  float GyX;
  float GyY;
  float GyZ;
};

struct acelerometroamostra {
  float AcX;
  float AcY;
  float AcZ;
  unsigned long millisamostra;
};

struct acelerometro {
  float AcX;
  float AcY;
  float AcZ;
};

float xval[100] = {0};
float yval[100] = {0};
float zval[100] = {0};
float xavg;
float yavg;
float zavg;

//## Variáveis para reconhecimento de passos
float maximo = 0;
float minimo = 100;
float threshold = 50;
float step_sample[2];

float precision = 1;
bool flag1;
bool flag2;
int passos = 0;

//#####
//###############################  ARDUINO  ########################################
void setup()
{
  Serial.println("################  Inicia Setup  ################");
  iniciaSerial(false);  delay(100);
  ligaLed(ledvermelho, ledverde);  delay(100);
  iniciaWifi();  delay(100);
  piscaLed(ledvermelho, 200, 1);  delay(100);
  iniciaTempo();  delay(100);
  piscaLed(ledvermelho, 200, 1);  delay(100);
  Wire.begin();  delay(100);
  piscaLed(ledvermelho, 200, 1);  delay(100);
  iniciaMPU(MPU);  delay(100);
  piscaLed(ledvermelho, 200, 1);  delay(100);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);  delay(100);

  piscaLeds(ledvermelho, ledverde, 100, 3);
  //millisinicio = millis();
  Serial.println("################  Fim do Setup  ################");
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  lerPassos();
  Serial.print("Passos:");
  Serial.println(passos);
  //Serial.println("Fim do Loop");
}
