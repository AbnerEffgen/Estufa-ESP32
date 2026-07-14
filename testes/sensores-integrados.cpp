#include <WiFi.h>
#include <time.h>
#include <DHT.h>

//====================================================
// WIFI
//====================================================

const char *ssid = "";
const char *password = "";

//====================================================
// PINOS
//====================================================

#define DHTPIN 4

#define TRIG_PIN 18
#define ECHO_PIN 19

#define LDR_PIN 34
#define SOLO_PIN 35

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

//====================================================
// Estrutura
//====================================================

struct LeituraSensores
{
    bool rtcOk;

    bool dhtOk;
    float temperatura;
    float umidade;

    bool hcOk;
    float distancia;

    int luminosidade;
    int umidadeSolo;

    time_t timestamp;
};

LeituraSensores leitura;

//====================================================
// WIFI
//====================================================

void conectarWiFi()
{
    Serial.print("Conectando ao WiFi");

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println(" OK");
}

//====================================================
// RTC
//====================================================

void iniciarRTC()
{
    configTime(-3 * 3600, 0,
               "pool.ntp.org",
               "time.nist.gov");

    Serial.print("Sincronizando relogio");

    while (time(nullptr) < 100000)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println(" OK");
}

//====================================================
// Sensores
//====================================================

void lerSensores()
{
    //---------------- RTC ----------------

    leitura.timestamp = time(nullptr);
    leitura.rtcOk = leitura.timestamp > 100000;

    //---------------- DHT22 ----------------

    leitura.temperatura = dht.readTemperature();
    leitura.umidade = dht.readHumidity();

    leitura.dhtOk =
        !(isnan(leitura.temperatura) ||
          isnan(leitura.umidade));

    //---------------- LDR ----------------

    leitura.luminosidade = analogRead(LDR_PIN);

    //---------------- Solo ----------------

    leitura.umidadeSolo = analogRead(SOLO_PIN);

    //---------------- HC-SR04 ----------------

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duracao == 0)
    {
        leitura.hcOk = false;
        leitura.distancia = -1;
    }
    else
    {
        leitura.hcOk = true;
        leitura.distancia = duracao * 0.0343 / 2.0;
    }
}

//====================================================
// LOG
//====================================================

void imprimirLog()
{
    Serial.println();
    Serial.println("=========================================");

    if (leitura.rtcOk)
    {
        struct tm *tempo = localtime(&leitura.timestamp);

        Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n",
                      tempo->tm_mday,
                      tempo->tm_mon + 1,
                      tempo->tm_year + 1900,
                      tempo->tm_hour,
                      tempo->tm_min,
                      tempo->tm_sec);
    }
    else
    {
        Serial.println("RTC: NAO CONECTADO");
    }

    Serial.println("-----------------------------------------");

    if (leitura.dhtOk)
    {
        Serial.printf("Temperatura   : %.2f C\n", leitura.temperatura);
        Serial.printf("Umidade Ar    : %.2f %%\n", leitura.umidade);
    }
    else
    {
        Serial.println("DHT22         : NAO CONECTADO");
    }

    Serial.printf("Luminosidade  : %d\n", leitura.luminosidade);
    Serial.printf("Umidade Solo  : %d\n", leitura.umidadeSolo);

    if (leitura.hcOk)
    {
        Serial.printf("Nivel Agua    : %.2f cm\n", leitura.distancia);
    }
    else
    {
        Serial.println("HC-SR04       : NAO CONECTADO");
    }

    Serial.println("=========================================");
}

//====================================================
// SETUP
//====================================================

void setup()
{
    Serial.begin(115200);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    analogReadResolution(12);

    dht.begin();

    conectarWiFi();

    iniciarRTC();
}

//====================================================
// LOOP
//====================================================

void loop()
{
    lerSensores();

    imprimirLog();

    delay(2000);
}