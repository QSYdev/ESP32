#pragma once
#include <Observer.hpp>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <list>
#include <Arduino.h>
#include <QSYBLEPacket.hpp>

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

	class BLEPacketParser : public BLECallback
	{
	private:
		char mHeader[QSY_BLE_HEADER_PACKET_SIZE];
		uint16_t mWritePos;
		char* mPayload;
		unsigned long mLastTimeReceived;

		SemaphoreHandle_t mMutex;
		void* mPacketReceived;
		QSYBLEHeaderPacket::PacketType mPacketType;

	public:
		inline BLEPacketParser(BluetoothReceiver* bluetooth, BLECharacteristic* characteristic)
			:BLECallback(bluetooth, characteristic), mWritePos(0), mPayload(nullptr), mLastTimeReceived(millis()), mMutex(xSemaphoreCreateMutex()), mPacketReceived(nullptr)
		{
		}

		void onWrite(BLECharacteristic* pCharacteristic) override;
		const Event* tick();

	private:
		void processPacket(const QSYBLEHeaderPacket* header, const void* payload);

	};

	GetConnectedNodes* mGetConnectedNodes;
	BLEPacketParser* mBLEPacketParser;

public:
	BluetoothReceiver();

	void init();
	void tick();

	void hello(uint16_t physicalId);
	void touche(uint16_t physicalId);
	void disconnectedNode(uint16_t phyisicalId);
	
};