#include <TCPSender.hpp>

void TCPSender::NewNodeTask::execute()
{

}

TCPSender::SendCommandTask::SendCommandTask(const qsy_packet* packet)
	:mPacket(packet)
{

}

void TCPSender::SendCommandTask::execute()
{

}

void TCPSender::DisconnectedNodeTask::execute()
{

}

TCPSender::TCPSender()
{

}

TCPSender::~TCPSender()
{

}

void TCPSender::tick()
{

}

void TCPSender::hello(uint16_t physicalId, WiFiClient* node)
{

}

void TCPSender::command(const qsy_packet* packet)
{

}

void TCPSender::disconnectedNode(uint16_t physicalId)
{

}