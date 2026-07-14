#include "controle.h"

#include <Arduino.h>

#include "config.h"
#include "dados.h"

//====================================================
// Inicialização
//====================================================

void iniciarAtuadores()
{
    pinMode(BOMBA_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(BOMBA_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
}

//====================================================
// Controle
//====================================================

void controlarEstufa()
{
    // Bomba
    if (leitura.umidadeSolo < 1800)
        digitalWrite(BOMBA_PIN, HIGH);
    else
        digitalWrite(BOMBA_PIN, LOW);

    // Ventoinha
    if (leitura.temperatura > 30)
        digitalWrite(FAN_PIN, HIGH);
    else
        digitalWrite(FAN_PIN, LOW);

    // Iluminação
    if (leitura.luminosidade < 1000)
        digitalWrite(LED_PIN, HIGH);
    else
        digitalWrite(LED_PIN, LOW);
}