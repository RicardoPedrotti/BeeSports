#include<Arduino.h>
#include<Wire.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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

int threshhold = 80;
int steps, flag = 0;

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

//###############################  ARDUINO  ########################################
void setup()
{
  Serial.println("################  Inicia Setup  ################");
  iniciaSerial();
  delay(500);
  iniciaWifi();
  delay(500);
  iniciaTempo();
  delay(500);
  Wire.begin();
  delay(500);
  iniciaMPU(MPU);
  delay(500);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(2000);
  //millisinicio = millis();
  Serial.println("################  Fim do Setup  ################");
}
void loop()
{
  int acc = 0;
  float totvect[100] = {0};
  float totave[100] = {0};
  //float sum1,sum2,sum3=0;
  float xaccl[100] = {0};
  float yaccl[100] = {0};
  float zaccl[100] = {0};

  for (int i = 0; i < 100; i++) {
    acelerometroamostra amostra;
    amostra = accel_filtrado('TRUE');
    //Define AcX
    xaccl[i] = amostra.AcX;
    delay(1);
    //Define AcY
    yaccl[i] = amostra.AcY;
    delay(1);
    //Define AcZ
    zaccl[i] = amostra.AcZ;
    delay(1);

    totvect[i] = sqrt(((xaccl[i] - xavg) * (xaccl[i] - xavg)) + ((yaccl[i] - yavg) * (yaccl[i] - yavg)) + ((zval[i] - zavg) * (zval[i] - zavg)));
    totave[i] = (totvect[i] + totvect[i - 1]) / 2 ;
    Serial.println(totave[i]);
    delay(200);

    
    //detectando os Steps
    if (totave[i] > threshhold && flag == 0)
    {
      steps = steps + 1;
      flag = 1;
    }
    else if (totave[i] > threshhold && flag == 1)
    {
      //do nothing
    }
    if (totave[i] < threshhold  && flag == 1)
    {
      flag = 0;
    }
    Serial.println('\n');
    Serial.print("steps=");
    Serial.println(steps);
  }
  delay(1000);
  Serial.println("Fim do Loop");
}
