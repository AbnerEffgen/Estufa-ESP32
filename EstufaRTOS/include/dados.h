#ifndef DADOS_H
#define DADOS_H

#include <Arduino.h>
#include <time.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

//====================================================
// Estrutura compartilhada
//====================================================

struct Telemetria
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

//====================================================
// Variáveis globais
//====================================================

extern Telemetria leitura;

extern SemaphoreHandle_t mutexTelemetria;

#endif