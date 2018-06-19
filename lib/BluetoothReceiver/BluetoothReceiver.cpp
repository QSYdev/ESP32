#include <BluetoothReceiver.hpp>
#include <QSYPacket.hpp>

BluetoothReceiver::BluetoothReceiver()
	:mMutexBuffer(xSemaphoreCreateMutex()), mReadPos(0), mWritePos(0)
{
}

void BluetoothReceiver::init()
{
	BLEDevice::init(QSY_BLUETOOTH);
	BLEServer* pServer = BLEDevice::createServer();
	BLEService* pService = pServer->createService(QSY_SERVICE_UUID);
	BLECharacteristic* pCharacteristic = pService->createCharacteristic(QSY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
	pCharacteristic->setCallbacks(new BluetoothReceiver::MyCallbacks(this));
	pService->start();
	BLEAdvertising* pAdvertising = pServer->getAdvertising();
	pAdvertising->start();
}

void BluetoothReceiver::MyCallbacks::onWrite(BLECharacteristic* pCharacteristic)
{
	xSemaphoreTake(mBluetooth->mMutexBuffer, portMAX_DELAY);
	{
		std::string value = pCharacteristic->getValue();
		for (int i = 0; i < value.size(); i++)
			mBluetooth->mBuffer[(mBluetooth->mWritePos++) % 64] = value[i];
	}
	xSemaphoreGive(mBluetooth->mMutexBuffer);
}

void BluetoothReceiver::tick()
{
	int data = -1;
	xSemaphoreTake(mMutexBuffer, portMAX_DELAY);
	{
		int available = mWritePos - mReadPos;
		if (available)
			data = mBuffer[(mReadPos++) % 64];
	}
	xSemaphoreGive(mMutexBuffer);

	if (data != -1)
	{
		CommandReceivedFromUser event(data);
		notify(&event);
	}
}