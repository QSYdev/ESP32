#include <WiFiManager.hpp>
#include <WiFi.h>
#include <QSYWiFiPacket.hpp>

WiFiManager::WiFiManager()
{
}

void WiFiManager::init()
{
	WiFi.mode(WIFI_AP);
	WiFi.softAPdisconnect(true);
	Serial.println("Desconectando...");
	vTaskDelay(5000);
	Serial.println("WiFi Conectado!");
	WiFi.softAP(QSY_WIFI_SSID, QSY_WIFI_PASSWORD, QSY_WIFI_CHANNEL, QSY_WIFI_SSID_HIDDEN, QSY_WIFI_MAX_CONNECTIONS);
}

void WiFiManager::tick()
{
}