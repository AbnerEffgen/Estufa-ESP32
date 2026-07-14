#include "log.h"

#include <Arduino.h>
#include <time.h>

#include "dados.h"

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
        Serial.printf("Temperatura : %.2f C\n", leitura.temperatura);
        Serial.printf("Umidade Ar  : %.2f %%\n", leitura.umidade);
    }
    else
    {
        Serial.println("DHT22       : NAO CONECTADO");
    }

    Serial.printf("Luminosidade : %d\n", leitura.luminosidade);
    Serial.printf("Umidade Solo : %d\n", leitura.umidadeSolo);

    if (leitura.hcOk)
    {
        Serial.printf("Nivel Agua   : %.2f cm\n", leitura.distancia);
    }
    else
    {
        Serial.println("HC-SR04      : NAO CONECTADO");
    }

    Serial.println("=========================================");
}