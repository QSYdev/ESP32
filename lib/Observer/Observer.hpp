#pragma once
#include <List.hpp>
#include <FreeRTOS.h>
#include <IPAddress.h>
#include <QSYPacket.hpp>

class Event
{

public:
	enum event_type
	{
		PacketReceived = 0,
	};

	const event_type mType;
	inline Event(const event_type type)	:mType(type) {}
	inline Event(const Event& event)	:mType(event.mType) {}
};

class PacketReceived : public Event
{

public:
	const IPAddress mIpRemote;
	const qsy_packet* mPacket;

	inline PacketReceived(const IPAddress ipRemote, const qsy_packet* packet)	:Event(event_type::PacketReceived), mIpRemote(ipRemote), mPacket(packet) {}
	inline PacketReceived(const PacketReceived& event)	:Event(event), mIpRemote(event.mIpRemote), mPacket(reinterpret_cast<const qsy_packet*>(new char[QSY_PACKET_SIZE]()))
	{
		char* buffer = reinterpret_cast<char*>(const_cast<qsy_packet*>(mPacket));
		for (int i = 0; i < QSY_PACKET_SIZE; i++)
			buffer[i] = event.mPacket->privated[i];
	}
};

class Observer
{

public:
	Observer();

	virtual void notify(const Event* event) = 0;

};

class SynchronousObserver : public Observer
{

public:
	SynchronousObserver();

};

class AsynchronousObserver : public Observer
{

private:
	List<const Event*> mEvents;
	SemaphoreHandle_t mMutex;
	SemaphoreHandle_t mData;

public:
	AsynchronousObserver();
	~AsynchronousObserver();

	void notify(const Event* event) override;
	const Event* getEvent();
};

class Observable
{

private:
	List<Observer*> mObservers;

public:
	Observable();

	void add(Observer* observer);
	void remove(Observer* observer);

protected:
	void notify(const Event* event);

};