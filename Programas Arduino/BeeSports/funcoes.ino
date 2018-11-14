//##################### FUNÇÕES ####################
const char* ssid = "Virus";
const char* password = "ric12345";

static void iniciaSerial()
{
  Serial.println("Booting in 115200");
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Serial Iniciado com Sucesso.");
}

void iniciaWifi() {
  WiFi.mode(WIFI_STA);
  delay(5000);
  WiFi.begin(ssid, password);
  //Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    piscaLed(ledvermelho, 10, 5);
    Serial.print(".");
    Serial.println("");
    delay(1000);
  }
  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void piscaCheckWifi(){
  if (WiFi.status() != WL_CONNECTED){
    piscaLed(ledvermelho, 10, 5);
  }
}

static void iniciaTempo() {
  time_t epochTime;
  int timezone = -3;
  int dst = 0;
  configTime(timezone * 3600, dst, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nConectando aos sites de Tempo");
  while (true) {
    epochTime = time(NULL);
    int MIN_EPOCH = 3600;

    if (epochTime < MIN_EPOCH) {
      Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
      delay(2000);
    } else {
      Serial.print("Fetched NTP epoch time is: ");
      Serial.println(epochTime);
      break;
    }
    Serial.println("Conectado ao tempo!");
  }
}

String calculaHorario() {
  //Começando as funções de tempo
  time_t now;
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);
  String str_horario = "";

  //Prints de Data
  int dia = timeinfo->tm_mday;
  int mes = timeinfo->tm_mon + 1;
  int ano = timeinfo->tm_year + 1900;
  int hora = timeinfo->tm_hour;
  int minutos = timeinfo->tm_min;
  int segundos = timeinfo->tm_sec;

  str_horario = str_horario + dia + "/" + mes + "/" + ano;
  str_horario = str_horario + " " + hora + ":" + minutos + ":" + segundos;
  return str_horario;
}

void ligaLed(int led1, int led2) {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  delay(3000);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}

void piscaLed(int led, int tempo, int qtd) {
  for (int i = 0; i <= qtd; i++) {
    digitalWrite(led, HIGH);
    delay(tempo);
    digitalWrite(led, LOW);
  }
}

void piscaLeds(int led1, int led2, int tempo, int qtd) {
  for (int i = 0; i <= qtd; i++) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(tempo);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
  }
}
//#################################################################################
