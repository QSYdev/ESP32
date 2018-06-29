#include <QSYBLEPacket.hpp>
#include <arpa/inet.h>

#define RED_COMPONENT(x)   (x) >> 12 & 0xF
#define GREEN_COMPONENT(x) (x) >> 8  & 0xF
#define BLUE_COMPONENT(x)  (x) >> 4  & 0xF
#define COLOR(r,g,b) ((((r) & 0xF) << 12) | (((g) & 0xF) << 8) | (((b) & 0xF) << 4))

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

uint16_t QSYBLECommandRequestPacket::getId() const
{
	return (uint16_t) ntohs(mId);
}

const Color QSYBLECommandRequestPacket::getColor() const
{
	uint16_t hcolor = ntohs(mColor);
	Color result(RED_COMPONENT(hcolor), GREEN_COMPONENT(hcolor), BLUE_COMPONENT(hcolor));
	return result;
}

uint32_t QSYBLECommandRequestPacket::getDelay() const
{
	return ntohl(mDelay);
}

uint16_t QSYBLECommandRequestPacket::getStep() const
{
	return ntohs(mStep);
}