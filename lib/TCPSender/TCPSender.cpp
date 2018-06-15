#include <TCPSender.hpp>

TCPSender::TCPSender()
	:mConnectedNodes(), mSemConnectedNodes(xSemaphoreCreateMutex()), mPendingTasks(), mSemPendingTasks(xSemaphoreCreateMutex()), mSemAvailableData(xSemaphoreCreateCounting(0, 100))
{
}

void TCPSender::init()
{
}

void TCPSender::tick()
{
	try
	{
		const qsy_packet* packet = nullptr;
		xSemaphoreTake(mSemAvailableData, portTICK_PERIOD_MS);
		xSemaphoreTake(mSemPendingTasks, portTICK_PERIOD_MS);
		{
			packet = mPendingTasks.removeFirst();
		}
		xSemaphoreGive(mSemPendingTasks);

		WiFiClient* client = nullptr;
		xSemaphoreTake(mSemConnectedNodes, portTICK_PERIOD_MS);
		{
			client = mConnectedNodes.findById(packet_get_id(packet));
		}
		xSemaphoreGive(mSemConnectedNodes);

		if (packet && client && packet_is_valid(packet) && packet_get_type(packet) == packet_type::command)
		{
			client->write(reinterpret_cast<const char*>(packet), QSY_PACKET_SIZE);
			delete packet;
		}
	}
	catch (...)
	{

	}
}

void TCPSender::hello(uint16_t physicalId, WiFiClient* node)
{
	xSemaphoreTake(mSemConnectedNodes, portTICK_PERIOD_MS);
	{
		mConnectedNodes.addById(physicalId, node);
	}
	xSemaphoreGive(mSemConnectedNodes);
}

void TCPSender::command(const qsy_packet* packet)
{
	xSemaphoreTake(mSemPendingTasks, portTICK_PERIOD_MS);
	{
		mPendingTasks.add(new qsy_packet(packet));
	}
	xSemaphoreGive(mSemPendingTasks);
	xSemaphoreGive(mSemAvailableData);
}

void TCPSender::disconnectedNode(uint16_t physicalId)
{
	xSemaphoreTake(mSemConnectedNodes, portTICK_PERIOD_MS);
	{
		mConnectedNodes.removeById(physicalId);
	}
	xSemaphoreGive(mSemConnectedNodes);
}