#pragma once
#include <Observer.hpp>

class DeadNodesPurger : public Observable
{

private:
	class ListElement
	{

	public:
		const uint16_t mPhysicalId;
		unsigned long mLastKeepAlive;
		uint8_t mTries;
	
		inline ListElement(uint16_t physicalId, unsigned long lastKeepAlive)	:mPhysicalId(physicalId), mLastKeepAlive(lastKeepAlive), mTries(0)	{}
	};

private:
	List<ListElement*> mConnectedNodes;
	unsigned long mElapsedTime;

public:
	DeadNodesPurger();

	void init();
	void tick();

	void hello(uint16_t physicalId);
	void touche(uint16_t physicalId);
	void keepAlive(uint16_t physicalId);
	void disconnectedNode(uint16_t physicalId);

};