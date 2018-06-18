#pragma once
#include <Observer.hpp>
#include <BluetoothSerial.h>

class BluetoothReceiver : public Observable
{

private:
	BluetoothSerial mSerialBT;

public:
	BluetoothReceiver();

	void init();
	void tick();
	
};