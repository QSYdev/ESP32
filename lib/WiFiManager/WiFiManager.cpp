#include <WiFiManager.hpp>
#include <WiFi.h>
#include <QSYPacket.hpp>

WiFiManager::WiFiManager()
{
}

void WiFiManager::init()
{
	WiFi.mode(WIFI_AP);
	WiFi.softAP(QSY_SSID, QSY_PASSWORD, 1, 1, QSY_MAX_CONNECTIONS);
}

void WiFiManager::tick()
{
	
}