#pragma once
#include <List.hpp>
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
	inline Event(event_type type)	:mType(type) {}
};

class PacketReceived : public Event
{

public:
	const IPAddress mIpRemote;
	const qsy_packet* mPacket;

	inline PacketReceived(IPAddress ipRemote, qsy_packet* packet)	:Event(event_type::PacketReceived), mIpRemote(ipRemote), mPacket(packet) {}
};

class Observer
{

public:
	Observer();

	virtual void notify(Event* event) = 0;

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
	void notify(Event* event);

};