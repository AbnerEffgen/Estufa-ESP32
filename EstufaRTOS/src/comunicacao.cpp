#include "comunicacao.h"

#include <WiFi.h>
#include <time.h>

#include "config.h"

//====================================================
// WIFI
//====================================================

void conectarWiFi()
{
    Serial.print("Conectando ao WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println(" OK");
}

//====================================================
// RTC (NTP)
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
// Mantém WiFi conectado
//====================================================

void manterConexao()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi desconectado.");

        conectarWiFi();

        iniciarRTC();
    }
}