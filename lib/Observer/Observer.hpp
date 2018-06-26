#pragma once
#include <list>
#include <IPAddress.h>
#include <QSYWiFiPacket.hpp>
#include <Routine.hpp>

class Event
{

public:
	enum EventType
	{
		PacketReceived = 0,
		DisconnectedNode,
		CommandRequest,
		StartCustomExecution,
		StartPlayerExecution,
		StopExecution,
	};

	const EventType mType;
	inline Event(const EventType type)	:mType(type) {}

};

class PacketReceived : public Event
{

public:
	const IPAddress mIpRemote;
	const QSYWiFiPacket* const mPacket;

	inline PacketReceived(const IPAddress ipRemote, const QSYWiFiPacket* packet)	:Event(EventType::PacketReceived), mIpRemote(ipRemote), mPacket(packet) {}

};

class DisconnectedNode : public Event
{

public:
	const uint16_t mPhysicalId;

	inline DisconnectedNode(uint16_t physicalId)	:Event(EventType::DisconnectedNode), mPhysicalId(physicalId)	{}
};

class CommandRequest : public Event
{

public:
	const uint16_t mId;
	const Color mColor;
	const uint32_t mDelay;
	const uint16_t mStep;

	inline CommandRequest(uint16_t id, const Color& color, uint32_t delay, uint16_t step)	:Event(EventType::CommandRequest), mId(id), mColor(color), mDelay(delay), mStep(step)	{}

};

class StartCustomExecution : public Event
{

public:
	const Routine* const mRoutine;
	std::list<uint16_t> mAssociationsNodes;

	inline StartCustomExecution(Routine* routine, std::list<uint16_t>& associationsNodes)		:Event(EventType::StartCustomExecution), mRoutine(routine), mAssociationsNodes(associationsNodes)	{}
};

class StartPlayerExecution : public Event
{

public:
	std::list<Color> mPlayersAndColors;
	const bool mWaitForAllPlayers;
	const unsigned long mStepTimeOut;
	const unsigned long mStepDelay;
	const uint16_t mStepsCount;
	const bool mStopOnStepTimeOut;
	const unsigned long mExecutionTimeOut;
	std::list<uint16_t> mAssociationsNodes;

	inline StartPlayerExecution(std::list<Color>& playersAndColors, bool waitForAllPlayers, unsigned long stepTimeOut, unsigned long stepDelay, uint16_t stepsCount, bool stopOnStepTimeOut, unsigned long executionTimeOut, std::list<uint16_t>& associationsNodes)		
		:Event(EventType::StartPlayerExecution), mPlayersAndColors(playersAndColors), mWaitForAllPlayers(waitForAllPlayers), mStepTimeOut(stepTimeOut), mStepDelay(stepDelay), mStepsCount(stepsCount), mStopOnStepTimeOut(stopOnStepTimeOut), mExecutionTimeOut(executionTimeOut), mAssociationsNodes(associationsNodes)
	{
	}
};

class StopExecution : public Event
{

public:
	inline StopExecution()	:Event(EventType::StopExecution)	{}
};


class Observer
{

public:
	Observer();

	virtual void notify(const Event* event) = 0;

};

class Observable
{

private:
	std::list<Observer*> mObservers;

public:
	Observable();

	void add(Observer* observer);
	void remove(Observer* observer);

protected:
	void notify(const Event* event);

};