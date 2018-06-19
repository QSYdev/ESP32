#pragma once
#include <Observer.hpp>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <list>

class BluetoothReceiver : public Observable
{

private:
	class MyCallbacks: public BLECharacteristicCallbacks
	{
	private:
		BluetoothReceiver* mBluetooth;

    public:
		inline MyCallbacks(BluetoothReceiver* bluetooth)	:mBluetooth(bluetooth)	{}
		
		void onWrite(BLECharacteristic* pCharacteristic) override;
	
	};

	volatile SemaphoreHandle_t mMutexBuffer;
	volatile char mBuffer[64];
	volatile uint8_t mReadPos;
	volatile uint8_t mWritePos;

public:
	BluetoothReceiver();

	void init();
	void tick();
	
};