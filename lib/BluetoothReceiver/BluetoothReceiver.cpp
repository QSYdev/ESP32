#include <BluetoothReceiver.hpp>
#include <QSYPacket.hpp>

BluetoothReceiver::BluetoothReceiver()
	:mGetConnectedNodes(nullptr)
{
}

void BluetoothReceiver::init()
{
	BLEDevice::init(QSY_BLUETOOTH);
	BLEServer* pServer = BLEDevice::createServer();
	BLEService* pService = pServer->createService(QSY_SERVICE_UUID);

	/* GetConnectedNodes */
	BLECharacteristic* pCharacteristic = pService->createCharacteristic(QSY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
	pCharacteristic->setCallbacks(mGetConnectedNodes = new BluetoothReceiver::GetConnectedNodes(this, pCharacteristic));

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
	pCharacteristic->setValue("" + value);
}

void BluetoothReceiver::GetConnectedNodes::hello(uint16_t physicalId)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	value = ++mValue;
	xSemaphoreGive(mMutex);
	mCharacteristic->setValue("" + value);
	mCharacteristic->notify();
}

void BluetoothReceiver::GetConnectedNodes::disconnectedNode(uint16_t physicalId)
{
	uint16_t value = 0;
	xSemaphoreTake(mMutex, portMAX_DELAY);
	value = --mValue;
	xSemaphoreGive(mMutex);
	mCharacteristic->setValue("" + value);
	mCharacteristic->notify();
}

void BluetoothReceiver::tick()
{

}

void BluetoothReceiver::hello(uint16_t physicalId)
{
	mGetConnectedNodes->hello(physicalId);
}

void BluetoothReceiver::disconnectedNode(uint16_t physicalId)
{
	mGetConnectedNodes->disconnectedNode(physicalId);
}