#include <WiFiManager.hpp>
#include <WiFi.h>
#include <QSYPacket.hpp>

WiFiManager::WiFiManager()
{
}

void WiFiManager::init()
{
	WiFi.softAPdisconnect(true);
	WiFi.mode(WIFI_AP);
	WiFi.softAP(QSY_SSID, QSY_PASSWORD, 10, 1, QSY_MAX_CONNECTIONS);
}

void WiFiManager::tick()
{
	
}