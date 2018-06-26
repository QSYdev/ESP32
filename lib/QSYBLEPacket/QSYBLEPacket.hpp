#pragma once
#include <stdint.h>

#define QSY_BLE_SSID						"ESP32"
#define QSY_BLE_SERVICE_UUID				"4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define QSY_BLE_GET_CONNECTED_NODES_UUID	"beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define QSY_BLE_PACKET_PARSER_UUID			"68ff842d-1f89-4346-a4cc-ffd7a733acce"
#define QSY_BLE_HEADER_PACKET_SIZE			6

class QSYBLEHeaderPacket
{
private:
	char mSignature[3];
	uint8_t mType;
	uint16_t mPayloadSize;

public:
	enum PacketType
	{
		CommandRequest = 0,
		StartCustomExecution = 1,
		StartPlayerExecution = 2,
		StopExecution = 3,
	};

public:
	QSYBLEHeaderPacket();

	bool isValid() const;
	PacketType getType() const;
	uint16_t getPayloadSize() const;

} __attribute__ ((packed));