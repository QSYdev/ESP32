#include <QSYBLEPacket.hpp>
#include <arpa/inet.h>

QSYBLEHeaderPacket::QSYBLEHeaderPacket()
{
	mSignature[0] = 'Q';
	mSignature[1] = 'S';
	mSignature[2] = 'Y';
}

bool QSYBLEHeaderPacket::isValid() const
{
	return mSignature[0] == 'Q' && mSignature[1] == 'S' && mSignature[2] == 'Y' && mType >= 0 && mType <= 3;
}

QSYBLEHeaderPacket::PacketType QSYBLEHeaderPacket::getType() const
{
	return (PacketType) mType;
}

uint16_t QSYBLEHeaderPacket::getPayloadSize() const
{
	return (uint16_t) ntohs(mPayloadSize);
}