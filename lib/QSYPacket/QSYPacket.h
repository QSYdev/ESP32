#pragma once
#include <stdint.h>
#include <stdbool.h>

#define QSY_SSID				"ESP32"
#define QSY_PASSWORD			"123456789"
#define QSY_MAX_CONNECTIONS		8
#define QSY_PACKET_SIZE			16
#define QSY_MULTICAST_ADDRESS	{225, 0, 0, 12}
#define QSY_MULTICAST_PORT		3000
#define QSY_TCP_SERVER_PORT		3000

struct qsy_packet {
	char privated[QSY_PACKET_SIZE];
} __attribute__ ((packed));

enum packet_type {
	hello = 0,
	command = 1,
	touche = 2,
	keepalive = 3
};

struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} __attribute__ ((packed));

void packet_init(struct qsy_packet* packet);
bool packet_is_valid(const struct qsy_packet* packet);

enum packet_type packet_get_type(struct qsy_packet* packet);
uint16_t packet_get_id(const struct qsy_packet* packet);
struct color packet_get_color(const struct qsy_packet* packet);
uint32_t packet_get_delay(const struct qsy_packet* packet);
uint16_t packet_get_step(const struct qsy_packet* packet);

void packet_set_type(struct qsy_packet* packet, enum packet_type type);
void packet_set_id(struct qsy_packet* packet, uint16_t id);
void packet_set_color(struct qsy_packet* packet, struct color c);
void packet_set_delay(struct qsy_packet* packet, uint32_t delay);
void packet_set_step(struct qsy_packet* packet, uint16_t step);