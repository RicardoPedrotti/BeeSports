const float MPU_GYRO_250_SCALE = 131.0;
const float MPU_GYRO_500_SCALE = 65.5;
const float MPU_GYRO_1000_SCALE = 32.8;
const float MPU_GYRO_2000_SCALE = 16.4;
const float MPU_ACCL_2_SCALE = 16384.0;
const float MPU_ACCL_4_SCALE = 8192.0;
const float MPU_ACCL_8_SCALE = 4096.0;
const float MPU_ACCL_16_SCALE = 2048.0;

//############ FUNÇÕES DE DEFINIÇÃO DE ESCALAS
void setMPU6050scales(byte addr, uint8_t Gyro, uint8_t Accl) {
  Wire.beginTransmission(addr);
  Wire.write(0x1B); // write to register starting at 0x1B
  Wire.write(Gyro); // Self Tests Off and set Gyro FS to 250
  Wire.write(Accl); // Self Tests Off and set Accl FS to 8g
  Wire.endTransmission(true);
}

void getMPU6050scales(byte addr, uint8_t &Gyro, uint8_t &Accl) {
  Wire.beginTransmission(addr);
  Wire.write(0x1B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(addr, 2, true); // request a total of 14 registers
  Gyro = (Wire.read() & (bit(3) | bit(4))) >> 3;
  Accl = (Wire.read() & (bit(3) | bit(4))) >> 3;
}
//#######################################################

//##############  Função que lê os dados crus de 16 bits do sensor
rawdata lerMPU(byte addr, bool Debug) {
  rawdata values;
  Wire.beginTransmission(addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor (14 registros)
  Wire.requestFrom(MPU, 14, true);
  //Armazena o valor dos sensores nas variaveis correspondentes
  values.AcX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  values.AcY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  values.AcZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  values.Tmp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  values.GyX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  values.GyY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  values.GyZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  return values;
}

//####################### Função que recebe os dados brutos de rawdata e converte para valores escalares
scaleddata converteEscalar(byte addr, rawdata data_in, bool Debug) {
  scaleddata scalevalues;
  float scale_value = 0.0;
  byte Gyro, Accl;

  getMPU6050scales(MPU, Gyro, Accl);

  switch (Gyro) {
    case 0:
      scale_value = MPU_GYRO_250_SCALE;
      break;
    case 1:
      scale_value = MPU_GYRO_500_SCALE;
      break;
    case 2:
      scale_value = MPU_GYRO_1000_SCALE;
      break;
    case 3:
      scale_value = MPU_GYRO_2000_SCALE;
      break;
    default:
      break;
  }

  scalevalues.GyX = (float) data_in.GyX / scale_value;
  scalevalues.GyY = (float) data_in.GyY / scale_value;
  scalevalues.GyZ = (float) data_in.GyZ / scale_value;

  scale_value = 0.0;
  switch (Accl) {
    case 0:
      scale_value = MPU_ACCL_2_SCALE;
      break;
    case 1:
      scale_value = MPU_ACCL_4_SCALE;
      break;
    case 2:
      scale_value = MPU_ACCL_8_SCALE;
      break;
    case 3:
      scale_value = MPU_ACCL_16_SCALE;
      break;
    default:
      break;
  }
  scalevalues.AcX = (float) data_in.AcX / scale_value;
  scalevalues.AcY = (float) data_in.AcY / scale_value;
  scalevalues.AcZ = (float) data_in.AcZ / scale_value;

  scalevalues.Tmp = (float) data_in.Tmp / 340.0 + 36.53;

  if (Debug) {
    Serial.print(" GyX = "); Serial.print(scalevalues.GyX);
    Serial.print(" Â°/s| GyY = "); Serial.print(scalevalues.GyY);
    Serial.print(" Â°/s| GyZ = "); Serial.print(scalevalues.GyZ);
    Serial.print(" Â°/s| Tmp = "); Serial.print(scalevalues.Tmp);

    Serial.print(" Â°C| AcX = "); Serial.print(scalevalues.AcX);
    Serial.print(" g| AcY = "); Serial.print(scalevalues.AcY);
    Serial.print(" g| AcZ = "); Serial.print(scalevalues.AcZ); Serial.println(" g");
  }

  return scalevalues;
}
//####################################################################################

//#################### Funções de Passômetro
//### Essa função pega 3 leituras do MPU já convertidas para escalar e filtra só o accel
acelerometroamostra filtroDigital(scaleddata arrayamostra[], bool debug) {
  //Serial.println("Fazendo filtro digital...");
  //Calcula a média das 3 leituras e joga em uma variável
  acelerometroamostra accfiltrado;
  accfiltrado.AcX = (arrayamostra[1].AcX + arrayamostra[2].AcX + arrayamostra[3].AcX) / 3;
  accfiltrado.AcY = (arrayamostra[1].AcY + arrayamostra[2].AcY + arrayamostra[3].AcY) / 3;
  accfiltrado.AcZ = (arrayamostra[1].AcZ + arrayamostra[2].AcZ + arrayamostra[3].AcZ) / 3;
  accfiltrado.millisamostra = millis();
  if (debug) {
    Serial.print(" AcX_filtrado: "); Serial.print(accfiltrado.AcX);
    Serial.print(" | AcY_filtrado: "); Serial.print(accfiltrado.AcY);
    Serial.print(" | AcZ_filtrado: "); Serial.println(accfiltrado.AcZ);
    Serial.print(" | Millis: "); Serial.println(accfiltrado.millisamostra);
  }
  return accfiltrado;
}

acelerometroamostra accel_filtrado(bool debug) {
  acelerometroamostra acc_filtrado;
  scaleddata amostraescalar_media[3];
  for (int i_media = 1; i_media < 4; i_media++) {
    rawdata amostra;
    amostra = lerMPU(MPU, false);
    amostraescalar_media[i_media] = converteEscalar(MPU, amostra, false);
  }
  //### Filtro Digital
  acc_filtrado = filtroDigital(amostraescalar_media, debug);
  return acc_filtrado;
}

//Função que compara o que está salvo como máximo e mínimo
acelerometro accelMax(acelerometroamostra amostra, bool debug, acelerometro amostramax) {
  if (debug) {
    Serial.println("Checando Máximos");
  }
  acelerometro valoresmax;
  if (amostra.AcX >= amostramax.AcX) {
    valoresmax.AcX = amostra.AcX;
  }
  //Max e min de Y
  if (amostra.AcY >= amostramax.AcY) {
    valoresmax.AcY = amostra.AcY;
  }
  //Max e min de Z
  if (amostra.AcZ >= amostramax.AcZ) {
    valoresmax.AcZ = amostra.AcZ;
  }
  return valoresmax;
}

acelerometro accelMin(acelerometroamostra amostra, bool debug, acelerometro amostramin) {
  if (debug) {
    Serial.println("Checando Mínimos");
  }
  acelerometro valoresmin;
  if (amostra.AcX < amostramin.AcX) {
    valoresmin.AcX = amostra.AcX;
  }
  //Max e min de Y
  if (amostra.AcY < amostramin.AcY) {
    valoresmin.AcY = amostra.AcY;
  }
  //Max e min de Z
  if (amostra.AcZ < amostramin.AcZ) {
    valoresmin.AcZ = amostra.AcZ;
  }
  return valoresmin;
}

acelerometro limiteDinamico(acelerometro amostramax, acelerometro amostramin, bool debug) {
  acelerometro limitedinamico;
  limitedinamico.AcX = (amostramax.AcX + amostramin.AcX) / 2;
  limitedinamico.AcY = (amostramax.AcY + amostramin.AcY) / 2;
  limitedinamico.AcZ = (amostramax.AcZ + amostramax.AcZ) / 2;
  if (debug) {
    Serial.print(" Limite Dinâmico X: "); Serial.print(limitedinamico.AcX);
    Serial.print(" | Limite Dinâmico Y: "); Serial.print(limitedinamico.AcY);
    Serial.print(" | Limite Dinâmico Z: "); Serial.println(limitedinamico.AcZ);
  }
  return limitedinamico;
}

int lerPassos() {
  int acc = 0;
  float totvect[100] = {0};
  float totave[100] = {0};
  float xaccl[50] = {0};
  float yaccl[50] = {0};
  float zaccl[50] = {0};

  float minimo50 = 100;
  float maximo50 = 0;

  int threshhold = 80;
  int steps, flag = 0;

  for (int i = 0; i < 50; i++) {
    acelerometroamostra amostra;
    amostra = accel_filtrado(false);
    //Define AcX
    xaccl[i] = amostra.AcX; delay(1);
    //Define AcY
    yaccl[i] = amostra.AcY; delay(1);
    //Define AcZ
    zaccl[i] = amostra.AcZ; delay(1);

    totvect[i] = sqrt(pow(xaccl[i], 2) + pow(yaccl[i], 2) + pow(zaccl[i], 2));

    if (totvect[i] < minimo50) {
      minimo50 = totvect[i];
    }

    if (totvect[i] > maximo50) {
      maximo50 = totvect[i];
    }
    delay(100);

    //Serial.println(" ");
    //Serial.print("total: ");
    //Serial.print(totvect[i]);
    //Serial.print(",");
    //Serial.print("threshold: ");
    //Serial.println(threshold);
    
    if (iloop != 1) {
      float absolute = abs(threshold - totvect[i]);
      if ((0.15 < abs(threshold - totvect[i])) && (abs(threshold - totvect[i]) < 0.35)){
        passos = passos + 1;
        Serial.println("Andando");
      }
      if (abs(threshold - totvect[i]) > 0.35) {
        passos = passos + 1;
        Serial.println("Correndo");
      }
    }
  }

  maximo = maximo50;
  minimo = minimo50;
  threshold = (maximo + minimo) / 2;
  return passos;
}

