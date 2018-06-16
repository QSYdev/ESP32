#pragma once
#include <List.hpp>
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