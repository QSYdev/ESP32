#include <TCPSender.hpp>

TCPSender::TCPSender()
	:mConnectedNodes(), mSemConnectedNodes(xSemaphoreCreateMutex()), mPendingTasks(), mSemPendingTasks(xSemaphoreCreateMutex()), mSemAvailableData(xSemaphoreCreateCounting(10, 0))
{
}

void TCPSender::init()
{
}

void TCPSender::tick()
{
	const QSYPacket* packet = nullptr;
	try
	{
		xSemaphoreTake(mSemAvailableData, portMAX_DELAY);
		xSemaphoreTake(mSemPendingTasks, portMAX_DELAY);
		{
			packet = mPendingTasks.front();
			mPendingTasks.pop_front();
		}
		xSemaphoreGive(mSemPendingTasks);

		WiFiClient* client = nullptr;
		xSemaphoreTake(mSemConnectedNodes, portMAX_DELAY);
		{
			client = mConnectedNodes[packet->getId()];
		}
		xSemaphoreGive(mSemConnectedNodes);

		if (packet && client && packet->isValid() && packet->getType() == QSYPacket::PacketType::Command)
			client->write(reinterpret_cast<const char*>(packet), QSY_PACKET_SIZE);
	}
	catch (...)
	{
	}
	delete packet;
}

void TCPSender::hello(uint16_t physicalId, WiFiClient* node)
{
	xSemaphoreTake(mSemConnectedNodes, portMAX_DELAY);
	{
		mConnectedNodes[physicalId] = node;
	}
	xSemaphoreGive(mSemConnectedNodes);
}

void TCPSender::command(const QSYPacket* packet)
{
	xSemaphoreTake(mSemPendingTasks, portMAX_DELAY);
	{
		mPendingTasks.push_back(packet);
	}
	xSemaphoreGive(mSemPendingTasks);
	xSemaphoreGive(mSemAvailableData);
}

void TCPSender::disconnectedNode(uint16_t physicalId)
{
	xSemaphoreTake(mSemConnectedNodes, portMAX_DELAY);
	{
		mConnectedNodes.erase(physicalId);
	}
	xSemaphoreGive(mSemConnectedNodes);
}