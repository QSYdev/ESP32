#include <BluetoothReceiver.hpp>
#include <QSYPacket.hpp>

BluetoothReceiver::BluetoothReceiver()
	:mSerialBT()
{
}

void BluetoothReceiver::init()
{
	mSerialBT.begin(QSY_BLUETOOTH);
}

void BluetoothReceiver::tick()
{
	if (mSerialBT.available())
	{
		int n = mSerialBT.read();
		CommandReceivedFromUser event(n);
		notify(&event);
	}
}