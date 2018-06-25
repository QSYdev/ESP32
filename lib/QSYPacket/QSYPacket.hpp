#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define QSY_SSID						"ESP32"
#define QSY_PASSWORD					"123456789"
#define QSY_BLUETOOTH					"ESP32"
#define QSY_SERVICE_UUID				"4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define QSY_GET_CONNECTED_NODES_CHAR	"beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define QSY_SEND_COMMAND_CHAR			"68ff842d-1f89-4346-a4cc-ffd7a733acce"
#define QSY_MAX_CONNECTIONS				8
#define QSY_PACKET_SIZE					16
#define QSY_MULTICAST_ADDRESS			{224, 0, 0, 12}
#define QSY_MULTICAST_PORT				3000
#define QSY_TCP_SERVER_PORT				3000
#define QSY_MAX_ALLOWED_TIME			750
#define QSY_MAX_TRIES					5
#define QSY_DEAD_NODES_PURGER_PERIOD	1125

class Color
{

public:
	const uint8_t mRed;
	const uint8_t mGreen;
	const uint8_t mBlue;

	inline Color()	:mRed(0), mGreen(0), mBlue(0)	{}
	inline Color(uint8_t red, uint8_t green, uint8_t blue)	:mRed(red), mGreen(green), mBlue(blue)	{}

} __attribute__ ((packed));

class QSYPacket
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

	QSYPacket();
	
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