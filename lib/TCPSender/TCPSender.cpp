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
	const qsy_packet* packet = nullptr;
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
			client = mConnectedNodes[packet_get_id(packet)];
		}
		xSemaphoreGive(mSemConnectedNodes);

		if (packet && client && packet_is_valid(packet) && packet_get_type(packet) == packet_type::command)
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

void TCPSender::command(const qsy_packet* packet)
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