#include <QSYPacket.h>
#include <string.h>
#include <arpa/inet.h>

#define RED_COMPONENT(x)   (x) >> 12 & 0xF
#define GREEN_COMPONENT(x) (x) >> 8  & 0xF
#define BLUE_COMPONENT(x)  (x) >> 4  & 0xF
#define COLOR(r,g,b) ((((r) & 0xF) << 12) | (((g) & 0xF) << 8) | (((b) & 0xF) << 4))

struct qsy_packet_p {
	char signature[3];
	uint8_t type;
	uint16_t id;
	uint16_t color;
	uint32_t delay;
	uint16_t step;
	uint16_t config;
} __attribute__ ((packed));

void packet_init(struct qsy_packet* packet)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	memset(p, 0, QSY_PACKET_SIZE);

	p->signature[0] = 'Q';
	p->signature[1] = 'S';
	p->signature[2] = 'Y';
}

bool packet_is_valid(const struct qsy_packet* packet)
{
	return packet->privated [0] == 'Q' && packet->privated [1] == 'S' &&
	    packet->privated [2] == 'Y';
}

enum packet_type packet_get_type(struct qsy_packet* packet)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	return (enum packet_type) p->type;
}

uint16_t packet_get_id(const struct qsy_packet* packet)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	return (uint16_t) ntohs(p->id);
}

struct color packet_get_color(const struct qsy_packet* packet)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	struct color res;
	uint16_t hcolor = ntohs(p->color);

	res.red = RED_COMPONENT(hcolor);
	res.green = GREEN_COMPONENT(hcolor);
	res.blue = BLUE_COMPONENT(hcolor);
	return res;
}

uint32_t packet_get_delay(const struct qsy_packet* packet)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	return ntohl(p->delay);
}

uint16_t packet_get_step(const struct qsy_packet *packet)
{
	struct qsy_packet_p *p = (struct qsy_packet_p *) packet;
	return ntohs(p->step);
}

void packet_set_type(struct qsy_packet* packet, enum packet_type type)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	p->type = (uint8_t) type;
}

void packet_set_id(struct qsy_packet* packet, uint16_t id)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	p->id = (uint16_t) htons(id);
}

void packet_set_color(struct qsy_packet* packet, struct color c)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	uint16_t hcolor = COLOR(c.red, c.green, c.blue);
	p->color = htons(hcolor);
}

void packet_set_delay(struct qsy_packet* packet, uint32_t delay)
{
	struct qsy_packet_p* p = (struct qsy_packet_p*) packet;
	p->delay = htonl(delay);
}

void packet_set_step(struct qsy_packet *packet, uint16_t step)
{
	struct qsy_packet_p *p = (struct qsy_packet_p *) packet;
	p->step = htons(step);
}