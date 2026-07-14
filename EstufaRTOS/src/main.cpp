#include <Arduino.h>

#include "config.h"
#include "dados.h"
#include "sensores.h"
#include "comunicacao.h"
#include "controle.h"
#include "log.h"

//====================================================
// Variáveis globais
//====================================================

Telemetria leitura;

SemaphoreHandle_t mutexTelemetria;

TaskHandle_t taskSensores;
TaskHandle_t taskControle;
TaskHandle_t taskComunicacao;
TaskHandle_t taskLog;

//====================================================
// TASK SENSORES
//====================================================

void taskSensoresFunction(void *pvParameters)
{
    while (true)
    {
        xSemaphoreTake(mutexTelemetria, portMAX_DELAY);

        lerSensores();

        xSemaphoreGive(mutexTelemetria);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//====================================================
// TASK CONTROLE
//====================================================

void taskControleFunction(void *pvParameters)
{
    while (true)
    {
        xSemaphoreTake(mutexTelemetria, portMAX_DELAY);

        controlarEstufa();

        xSemaphoreGive(mutexTelemetria);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//====================================================
// TASK COMUNICACAO
//====================================================

void taskComunicacaoFunction(void *pvParameters)
{
    while (true)
    {
        manterConexao();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//====================================================
// TASK LOG
//====================================================

void taskLogFunction(void *pvParameters)
{
    while (true)
    {
        xSemaphoreTake(mutexTelemetria, portMAX_DELAY);

        imprimirLog();

        xSemaphoreGive(mutexTelemetria);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//====================================================
// SETUP
//====================================================

void setup()
{
    Serial.begin(115200);

    iniciarSensores();

    conectarWiFi();

    iniciarRTC();

    iniciarAtuadores();

    mutexTelemetria = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(
        taskSensoresFunction,
        "Sensores",
        4096,
        NULL,
        2,
        &taskSensores,
        1);

    xTaskCreatePinnedToCore(
        taskControleFunction,
        "Controle",
        4096,
        NULL,
        2,
        &taskControle,
        1);

    xTaskCreatePinnedToCore(
        taskComunicacaoFunction,
        "Comunicacao",
        4096,
        NULL,
        1,
        &taskComunicacao,
        0);

    xTaskCreatePinnedToCore(
        taskLogFunction,
        "Log",
        4096,
        NULL,
        1,
        &taskLog,
        0);
}

//====================================================
// LOOP
//====================================================

void loop()
{
}