#include <QSYPacket.hpp>
#include <string.h>
#include <arpa/inet.h>

#define RED_COMPONENT(x)   (x) >> 12 & 0xF
#define GREEN_COMPONENT(x) (x) >> 8  & 0xF
#define BLUE_COMPONENT(x)  (x) >> 4  & 0xF
#define COLOR(r,g,b) ((((r) & 0xF) << 12) | (((g) & 0xF) << 8) | (((b) & 0xF) << 4))

QSYPacket::QSYPacket()
{
	mSignature[0] = 'Q';
	mSignature[1] = 'S';
	mSignature[2] = 'Y';
}

bool QSYPacket::isValid() const
{
	return mSignature[0] == 'Q' && mSignature[1] == 'S' && mSignature[2] == 'Y';
}

enum QSYPacket::PacketType QSYPacket::getType() const
{
	return (PacketType) mType;
}

uint16_t QSYPacket::getId() const
{
	return (uint16_t) ntohs(mId);
}

const Color QSYPacket::getColor() const
{
	uint16_t hcolor = ntohs(mColor);
	Color result(RED_COMPONENT(hcolor), GREEN_COMPONENT(hcolor), BLUE_COMPONENT(hcolor));
	return result;
}

uint32_t QSYPacket::getDelay() const
{
	return ntohl(mDelay);
}

uint16_t QSYPacket::getStep() const
{
	return ntohs(mStep);
}

void QSYPacket::setType(QSYPacket::PacketType type)
{
	mType = (uint8_t) type;
}

void QSYPacket::setId(uint16_t id)
{
	mId = (uint16_t) htons(id);
}

void QSYPacket::setColor(const Color& color)
{
	uint16_t hcolor = COLOR(color.mRed, color.mGreen, color.mBlue);
	mColor = htons(hcolor);
}

void QSYPacket::setDelay(uint32_t delay)
{
	mDelay = htonl(delay);
}

void QSYPacket::setStep(uint16_t step)
{
	mStep = htons(step);
}