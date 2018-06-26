#pragma once
#include <Observer.hpp>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <list>
#include <Arduino.h>

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

	class SendCommand : public BLECallback
	{
	private:
		SemaphoreHandle_t mMutex;
		char mBuffer[QSY_WIFI_PACKET_SIZE];
		uint8_t mWritePos;
		bool mReadyToRead;
		unsigned long mLastTimeReceived;

	public:
		inline SendCommand(BluetoothReceiver* bluetooth, BLECharacteristic* characteristic)	
			:BLECallback(bluetooth, characteristic), mMutex(xSemaphoreCreateMutex()), mWritePos(0), mReadyToRead(false), mLastTimeReceived(millis())
		{
		}

		void onWrite(BLECharacteristic* pCharacteristic) override;
		const QSYWiFiPacket* tick();
	};

	GetConnectedNodes* mGetConnectedNodes;
	SendCommand* mSendCommand;

public:
	BluetoothReceiver();

	void init();
	void tick();

	void hello(uint16_t physicalId);
	void disconnectedNode(uint16_t phyisicalId);
	
};