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
	WiFi.softAP(QSY_WIFI_SSID, QSY_WIFI_PASSWORD, 10, 1, QSY_WIFI_MAX_CONNECTIONS);
}

void WiFiManager::tick()
{
	
}