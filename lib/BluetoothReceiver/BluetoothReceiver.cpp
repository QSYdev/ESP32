#include <BluetoothReceiver.hpp>
#include <QSYWiFiPacket.hpp>
#include <QSYBLEPacket.hpp>

void BluetoothReceiver::GetConnectedNodes::onRead(BLECharacteristic* pCharacteristic)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	{
		value = mValue;
	}
	xSemaphoreGive(mMutex);
	uint8_t temp[2];
	temp[0] = value;
	temp[1] = value >> 8;
	pCharacteristic->setValue(temp, 2);
}

void BluetoothReceiver::GetConnectedNodes::hello(uint16_t physicalId)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	{
		value = mValue;
	}
	xSemaphoreGive(mMutex);
	uint8_t temp[2];
	temp[0] = value;
	temp[1] = value >> 8;
	mCharacteristic->setValue(temp, 2);
	mCharacteristic->notify();
}

void BluetoothReceiver::GetConnectedNodes::disconnectedNode(uint16_t physicalId)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	{
		value = mValue;
	}
	xSemaphoreGive(mMutex);
	uint8_t temp[2];
	temp[0] = value;
	temp[1] = value >> 8;
	mCharacteristic->setValue(temp, 2);
	mCharacteristic->notify();
}

void BluetoothReceiver::BLEPacketParser::onWrite(BLECharacteristic* pCharacteristic)
{
	unsigned long tNow = millis();
	if (tNow - mLastTimeReceived > 1000)
	{
		mWritePos = 0;
		delete[] mPayload;
		mPayload = nullptr;
	}
	mLastTimeReceived = tNow;

	const std::string& value = pCharacteristic->getValue();

	if (!mPayload)
	{
		for (int i = 0; i < value.size() && mWritePos < QSY_BLE_HEADER_PACKET_SIZE; i++)
			mHeader[mWritePos++] = value[i];
		
		if (mWritePos == QSY_BLE_HEADER_PACKET_SIZE)
		{
			const QSYBLEHeaderPacket* header = reinterpret_cast<const QSYBLEHeaderPacket*>(mHeader);
			if (header->isValid())
			{
				mPayload = new char[header->getPayloadSize()];

				for (int i = mWritePos - QSY_BLE_HEADER_PACKET_SIZE; i < header->getPayloadSize() && mWritePos < value.size(); i++)
					mPayload[i] = value[mWritePos++];

				if (mWritePos - QSY_BLE_HEADER_PACKET_SIZE == header->getPayloadSize())
				{
					processPacket(header, mPayload);
					mWritePos = 0;
					delete[] mPayload;
					mPayload = nullptr;
				}
			}
			else
			{
				mWritePos = 0;
			}
		}
	}
	else
	{
		const QSYBLEHeaderPacket* header = reinterpret_cast<const QSYBLEHeaderPacket*>(mHeader);

		int j = 0;
		for (int i = mWritePos - QSY_BLE_HEADER_PACKET_SIZE; i < header->getPayloadSize() && j < value.size(); i++)
		{
			mPayload[i] = value[j++];
			mWritePos++;
		}
		
		if (mWritePos - QSY_BLE_HEADER_PACKET_SIZE == header->getPayloadSize())
		{
			processPacket(header, mPayload);
			mWritePos = 0;
			delete[] mPayload;
			mPayload = nullptr;
		}
	}
}

void BluetoothReceiver::BLEPacketParser::processPacket(const QSYBLEHeaderPacket* header, const void* payload)
{
	xSemaphoreTake(mMutex, portMAX_DELAY);
	{
		mPacketType = header->getType();
		switch(mPacketType)
		{
			case QSYBLEHeaderPacket::PacketType::CommandRequest:
			{
				break;
			}

			case QSYBLEHeaderPacket::PacketType::StartCustomExecution:
			{
				break;
			}

			case QSYBLEHeaderPacket::PacketType::StartPlayerExecution:
			{
				break;
			}

			case QSYBLEHeaderPacket::PacketType::StopExecution:
			{
				break;
			}
		}
	}
	xSemaphoreGive(mMutex);
}

const Event* BluetoothReceiver::BLEPacketParser::tick()
{
	const Event* event = nullptr;
	if (xSemaphoreTake(mMutex, 10 / portTICK_PERIOD_MS) == pdTRUE)
	{
		if (mPacketReceived)
		{
			switch(mPacketType)
			{
				case QSYBLEHeaderPacket::PacketType::CommandRequest:
				{
					// event = new Event::EventType::CommandRequest();
					break;
				}

				case QSYBLEHeaderPacket::PacketType::StartCustomExecution:
				{
					// event = new Event::EventType::StartCustomExecution();
					break;
				}

				case QSYBLEHeaderPacket::PacketType::StartPlayerExecution:
				{
					// event = new Event::EventType::StartPlayerExecution();
					break;
				}

				case QSYBLEHeaderPacket::PacketType::StopExecution:
				{
					// event = new Event::EventType::StopExecution();
					break;
				}
			}

			delete mPacketReceived;
			mPacketReceived = nullptr;
		}
	}
	xSemaphoreGive(mMutex);

	return event;
}

BluetoothReceiver::BluetoothReceiver()
	:mGetConnectedNodes(nullptr), mBLEPacketParser(nullptr)
{
}

void BluetoothReceiver::init()
{
	BLEDevice::init(QSY_BLE_SSID);
	BLEServer* pServer = BLEDevice::createServer();
	BLEService* pService = pServer->createService(QSY_BLE_SERVICE_UUID);

	/* GetConnectedNodes */
	BLECharacteristic* getConnectedNodesChar = pService->createCharacteristic(QSY_BLE_GET_CONNECTED_NODES_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
	getConnectedNodesChar->setCallbacks(mGetConnectedNodes = new BluetoothReceiver::GetConnectedNodes(this, getConnectedNodesChar));
	uint8_t temp[2];
	temp[0] = 0;
	temp[1] = 0;
	getConnectedNodesChar->setValue(temp, 2);

	/* PacketParser */
	BLECharacteristic* packetParserChar = pService->createCharacteristic(QSY_BLE_PACKET_PARSER_UUID, BLECharacteristic::PROPERTY_WRITE);
	packetParserChar->setCallbacks(mBLEPacketParser = new BluetoothReceiver::BLEPacketParser(this, packetParserChar));

	pService->start();
	BLEAdvertising* pAdvertising = pServer->getAdvertising();
	pAdvertising->start();
}

void BluetoothReceiver::tick()
{
	const Event* event = mBLEPacketParser->tick();
	if (event)
	{
		notify(event);
		delete event;
	}
}

void BluetoothReceiver::hello(uint16_t physicalId)
{
	mGetConnectedNodes->hello(physicalId);
}

void BluetoothReceiver::touche(uint16_t physicalId)
{
	//TODO implementar.
}

void BluetoothReceiver::disconnectedNode(uint16_t physicalId)
{
	mGetConnectedNodes->disconnectedNode(physicalId);
}