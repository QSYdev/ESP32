#pragma once
#include <Observer.hpp>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <list>

class BluetoothReceiver : public Observable
{

private:
	class BLECallback : public BLECharacteristicCallbacks
	{
	protected:
		BluetoothReceiver* mBluetooth;
		BLECharacteristic* mCharacteristic;

	public:
		inline BLECallback(BluetoothReceiver* bluetooth, BLECharacteristic* characteristic)	:mBluetooth(bluetooth), mCharacteristic(characteristic)	{}
		inline ~BLECallback()	{ delete mCharacteristic; }
	};

	class GetConnectedNodes : public BLECallback
	{
	private:
		SemaphoreHandle_t mMutex;
		uint16_t mValue;

	public:
		inline GetConnectedNodes(BluetoothReceiver* bluetooth, BLECharacteristic* characteristic)	
			:BLECallback(bluetooth, characteristic), mMutex(xSemaphoreCreateMutex()), mValue(0)
		{
		}

		void onRead(BLECharacteristic* pCharacteristic) override;
		void hello(uint16_t physicalId);
		void disconnectedNode(uint16_t physicalId);

	};

	GetConnectedNodes* mGetConnectedNodes;

public:
	BluetoothReceiver();

	void init();
	void tick();

	void hello(uint16_t physicalId);
	void disconnectedNode(uint16_t phyisicalId);
	
};