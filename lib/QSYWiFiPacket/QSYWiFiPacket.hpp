#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define QSY_WIFI_SSID						"ESP32"
#define QSY_WIFI_PASSWORD					"123456789"
#define QSY_WIFI_MAX_CONNECTIONS			8
#define QSY_WIFI_PACKET_SIZE				16
#define QSY_WIFI_MULTICAST_ADDRESS			{224, 0, 0, 12}
#define QSY_WIFI_MULTICAST_PORT				3000
#define QSY_WIFI_TCP_SERVER_PORT			3000
#define QSY_WIFI_MAX_ALLOWED_TIME			750
#define QSY_WIFI_MAX_TRIES					5
#define QSY_WIFI_DEAD_NODES_PURGER_PERIOD	1125

class Color
{

public:
	const uint8_t mRed;
	const uint8_t mGreen;
	const uint8_t mBlue;

	inline Color()	:mRed(0), mGreen(0), mBlue(0)	{}
	inline Color(uint8_t red, uint8_t green, uint8_t blue)	:mRed(red), mGreen(green), mBlue(blue)	{}

} __attribute__ ((packed));

class QSYWiFiPacket
{

private:
	char mSignature[3];
	uint8_t mType;
	uint16_t mId;
	uint16_t mColor;
	uint32_t mDelay;
	uint16_t mStep;
	uint16_t mConfig;

public:
	enum PacketType
	{
		Hello = 0,
		Command = 1,
		Touche = 2,
		Keepalive = 3
	};

public:

	QSYWiFiPacket();
	
	bool isValid() const;

	enum PacketType getType() const;
	uint16_t getId() const;
	const Color getColor() const;
	uint32_t getDelay() const;
	uint16_t getStep() const;

	void setType(enum PacketType type);
	void setId(uint16_t id);
	void setColor(const Color& color);
	void setDelay(uint32_t delay);
	void setStep(uint16_t step);

} __attribute__ ((packed));