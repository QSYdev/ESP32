#include <BluetoothReceiver.hpp>
#include <QSYPacket.hpp>

BluetoothReceiver::BluetoothReceiver()
	:mGetConnectedNodes(nullptr), mSendCommand(nullptr)
{
}

void BluetoothReceiver::init()
{
	BLEDevice::init(QSY_BLUETOOTH);
	BLEServer* pServer = BLEDevice::createServer();
	BLEService* pService = pServer->createService(QSY_SERVICE_UUID);

	/* GetConnectedNodes */
	BLECharacteristic* getConnectedNodesChar = pService->createCharacteristic(QSY_GET_CONNECTED_NODES_CHAR, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
	getConnectedNodesChar->setCallbacks(mGetConnectedNodes = new BluetoothReceiver::GetConnectedNodes(this, getConnectedNodesChar));
	getConnectedNodesChar->setValue("0");

	/* SendCommand */
	BLECharacteristic* sendCommandChar = pService->createCharacteristic(QSY_SEND_COMMAND_CHAR, BLECharacteristic::PROPERTY_WRITE);
	sendCommandChar->setCallbacks(mSendCommand = new BluetoothReceiver::SendCommand(this, sendCommandChar));

	pService->start();
	BLEAdvertising* pAdvertising = pServer->getAdvertising();
	pAdvertising->start();
}

void BluetoothReceiver::GetConnectedNodes::onRead(BLECharacteristic* pCharacteristic)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	value = mValue;
	xSemaphoreGive(mMutex);
	pCharacteristic->setValue(value);
}

void BluetoothReceiver::GetConnectedNodes::hello(uint16_t physicalId)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	value = ++mValue;
	xSemaphoreGive(mMutex);
	mCharacteristic->setValue(value);
	mCharacteristic->notify();
}

void BluetoothReceiver::GetConnectedNodes::disconnectedNode(uint16_t physicalId)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	value = --mValue;
	xSemaphoreGive(mMutex);
	mCharacteristic->setValue(value);
	mCharacteristic->notify();
}

void BluetoothReceiver::SendCommand::onWrite(BLECharacteristic* pCharacteristic)
{
	xSemaphoreTake(mMutex, portMAX_DELAY);
	{
		const std::string& value = pCharacteristic->getValue();
		unsigned long tNow = millis();
		mWritePos = (tNow - mLastTimeReceived > 1000) ? 0 : mWritePos;
		mLastTimeReceived = tNow;

		for (int i = 0; i < value.size() && mWritePos < QSY_PACKET_SIZE; i++)
			mBuffer[mWritePos++] = value[i];
		
		mWritePos = mWritePos % QSY_PACKET_SIZE;
		mReadyToRead = !mWritePos;
	}
	xSemaphoreGive(mMutex);
}

const QSYPacket* BluetoothReceiver::SendCommand::tick()
{
	const QSYPacket* result = nullptr;
	if (xSemaphoreTake(mMutex, 10 / portTICK_PERIOD_MS) == pdTRUE)
	{
		if (mReadyToRead)
		{
			result = reinterpret_cast<QSYPacket*>(mBuffer);
			if (!result->isValid() || result->getType() != QSYPacket::PacketType::Command)
				result = nullptr;
			mReadyToRead = false;
		}
	}
	xSemaphoreGive(mMutex);
	return result;
}

void BluetoothReceiver::tick()
{
	const QSYPacket* packet = mSendCommand->tick();
	if (packet)
	{
		const CommandRequest commandRequestEvent(packet->getId(), packet->getColor(), packet->getDelay(), packet->getStep());
		notify(&commandRequestEvent);
	}
}

void BluetoothReceiver::hello(uint16_t physicalId)
{
	mGetConnectedNodes->hello(physicalId);
}

void BluetoothReceiver::disconnectedNode(uint16_t physicalId)
{
	mGetConnectedNodes->disconnectedNode(physicalId);
}