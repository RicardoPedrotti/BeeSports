#include<Arduino.h>
#include<Wire.h>
#include <time.h>
#include <ESP8266WiFi.h>

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
  delay(2000);
  //millisinicio = millis();
  Serial.println("################  Fim do Setup  ################");
}
void loop()
{
  //millisatual = millis(); //captura o tempo atual desde o começo do programa
  
  unsigned long millisamostra = 0;
  bool onoff = true;
  acelerometroamostra acc_filtrado[50];
  scaleddata amostraescalar_max[50];
  acelerometro acc_max;
  acelerometro acc_min;
  acelerometro acc_limite;
  float sample_new[50];
  float sample_old[50];
  bool teste1 = false;
  bool teste2 = false;
  int contador_passos_ant = 0;
  int contador_passos = 0;

  do {
  //Teste envia Json:
    String cd_serial_dispo = "BEE0001";
    int bpm = 54;
    int passos = 11;
    
    //Serial.println("Início do DO...");
    for (int i_amostra50 = 1; i_amostra50 < 51; i_amostra50++) {
      acc_max = acc_min;
      acc_min = acc_max;
      acc_filtrado[i_amostra50] = accel_filtrado(false);
      //### Max e min de X
      acc_max = accelMax(acc_filtrado[i_amostra50], false, acc_max);
      acc_min = accelMin(acc_filtrado[i_amostra50], false, acc_min);
      millisamostra = millis();
    //### Definindo o sample_old
      /*sample_old[i_amostra50] = sample_new[i_amostra50 - 1];
      //##Definindo o sample_new
      if (acc_filtrado[i_amostra50].AcX > 1.04) {
        sample_new[i_amostra50] = acc_filtrado[i_amostra50].AcX;
      } else {
        sample_new[i_amostra50] = sample_new[i_amostra50 - 1];
      }*/
      delay(20);
    }
    acc_limite = limiteDinamico(acc_max, acc_min, false);
    //Estrutura de decisão para a contagem de passos da última amostra
    //Serial.println("Entrando no novo for de 50...");
    for (int i_amostra50 = 1; i_amostra50 < 51; i_amostra50++ ) {
      if (sample_new[i_amostra50] < sample_old[i_amostra50] /*&& acc_filtrado[i_amostra50].AcX < acc_limite.AcX*/) {
        contador_passos_ant = contador_passos;
        contador_passos = contador_passos + 1;
      }
    }

    //if (contador_passos > contador_passos_ant) {
    Serial.print("Passos: "); Serial.println(contador_passos);
    criaJson(cd_serial_dispo, bpm, passos);
    delay(10);
  } while (onoff = true);
  Serial.println("Fim do Loop");
}
