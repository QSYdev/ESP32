#include <Terminal.hpp>

Terminal::Terminal()
	:mWiFiManager(this), mMulticast(this, QSY_PACKET_SIZE)
{
}

void Terminal::start()
{
	wifiManager.init(QSY_SSID QSY_PASSWORD, QSY_MAX_CONNECTIONS);
	multicast.init(QSY_MULTICAST_ADDRESS, QSY_MULTICAST_PORT);
}

void Terminal::tick()
{
	wifiManager.tick();
	multicast.tick();
}

void Terminal::searchNodes()
{
	multicast.setAcceptingPackets(true);
}

void Terminal::finalizeNodesSearching()
{
	multicast.setAcceptingPackets(false);
}

void Terminal::packetReceived(struct qsy_packet* packet, IPAddress* ipRemote = nullptr)
{
	if (!packet_is_valid(packet))
		return;

	switch(packet_get_type)
	{
		case packet_type::hello:
			if (ipRemote != nullptr)
			{
				Serial.print("<HELLO> NODE ID = ");
				Serial.print(packet_get_id(packet));
				Serial.print(" IP = ");
				Serial.println(*ipRemote);
			}
			break;

		case packet_type::command
			break;
		
		case packet_type::touche
			break;

		case packet_type::keepalive
			break;
	}
}