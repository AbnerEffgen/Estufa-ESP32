#include "sensores.h"

#include <Arduino.h>
#include <DHT.h>

#include "config.h"
#include "dados.h"

//====================================================
// DHT22
//====================================================

DHT dht(DHT_PIN, DHT_TYPE);

//====================================================
// Inicialização
//====================================================

void iniciarSensores()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    analogReadResolution(12);

    dht.begin();
}

//====================================================
// Leitura dos Sensores
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

    //---------------- Umidade Solo ----------------

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
        leitura.distancia = duracao * 0.0343f / 2.0f;
    }
}