//Função para escrever no MPU. 

const int GYRO_CONFIG =   0x1B; // registro de configuração do giroscópio
const int ACCEL_CONFIG =  0x1C; // registro de configuração do acelerômetro
const int PWR_MGMT_1 = 0x6B; // registro de enregia do acelerômetro (para ligar e desligar o MPU)

// Função que lê o MPU, aceitando um registro
uint8_t readRegMPU(uint8_t reg){      // aceita um registro como parâmetro
  uint8_t data;
  Wire.beginTransmission(MPU);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.endTransmission(false);          // termina transmissão mas continua com I2C aberto (envia STOP e START)
  Wire.requestFrom(MPU, 1);        // configura para receber 1 byte do registro escolhido acima
  data = Wire.read();                   // lê o byte e guarda em 'data'
  return data;                          //retorna 'data'
}

// Função que escreve no MPU, aceitando um registro
void writeRegMPU(int reg, int val){      //aceita um registro e um valor como parâmetro
  Wire.beginTransmission(MPU);     // inicia comunicação com endereço do MPU6050
  Wire.write(reg);                      // envia o registro com o qual se deseja trabalhar
  Wire.write(val);                      // escreve o valor no registro
  Wire.endTransmission(true);           // termina a transmissão
}

void setGyroScale()
{
  writeRegMPU(GYRO_CONFIG, 1);
  /* função para configurar as escalas do giroscópio
   registro da escala do giroscópio: 0x1B[4:3]
   0 é 250°/s

    FS_SEL  Full Scale Range
      0        ± 250 °/s      0b00000000
      1        ± 500 °/s      0b00001000
      2        ± 1000 °/s     0b00010000
      3        ± 2000 °/s     0b00011000
*/
}

void setAccelScale()
{
  writeRegMPU(ACCEL_CONFIG, 1);
  /* função para configurar as escalas do acelerômetro
   registro da escala do acelerômetro: 0x1C[4:3]
   0 é 250°/s

    AFS_SEL   Full Scale Range
      0           ± 2g            0b00000000
      1           ± 4g            0b00001000
      2           ± 8g            0b00010000
      3           ± 16g           0b00011000
*/
}

void setSleepOff()
{
  // escreve 0 no registro de gerenciamento de energia(0x68), colocando o sensor em o modo ACTIVE
  writeRegMPU(PWR_MGMT_1, 0);
}

//Checa se existe algum dispositivo conectado na porta mencionada)
bool checkI2C(byte addr) {
  Wire.beginTransmission(addr);

  if (Wire.endTransmission() == 0)
  {
    Serial.print("Dispositivo encontrado em 0x");
    Serial.println(addr, HEX);
    return true;
  }
  else
  {
    Serial.print("Dispositivo não encontrado em 0x");
    Serial.println(addr, HEX);
    return false;
  }
}

//Função que liga o MPU, inicia a escala do giroscópio e inicia a escala do acelerômetro.
void iniciaMPU(byte addr) {
  if (checkI2C(addr)) {
    setSleepOff();
    delay(50); //tempo para ligar o MPU
    setGyroScale();
    delay(50);
    setAccelScale();
    delay(50);
  }
}
