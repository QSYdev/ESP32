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
		xSemaphoreTake(mSemAvailableData, portTICK_PERIOD_MS);
		xSemaphoreTake(mSemPendingTasks, portTICK_PERIOD_MS);
		{
			packet = mPendingTasks.front();
			mPendingTasks.pop_front();
		}
		xSemaphoreGive(mSemPendingTasks);

		WiFiClient* client = nullptr;
		xSemaphoreTake(mSemConnectedNodes, portTICK_PERIOD_MS);
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
	xSemaphoreTake(mSemConnectedNodes, portTICK_PERIOD_MS);
	{
		mConnectedNodes[physicalId] = node;
	}
	xSemaphoreGive(mSemConnectedNodes);
}

void TCPSender::command(const qsy_packet* packet)
{
	xSemaphoreTake(mSemPendingTasks, portTICK_PERIOD_MS);
	{
		mPendingTasks.push_back(new qsy_packet(packet));
	}
	xSemaphoreGive(mSemPendingTasks);
	xSemaphoreGive(mSemAvailableData);
}

void TCPSender::disconnectedNode(uint16_t physicalId)
{
	xSemaphoreTake(mSemConnectedNodes, portTICK_PERIOD_MS);
	{
		mConnectedNodes.erase(physicalId);
	}
	xSemaphoreGive(mSemConnectedNodes);
}