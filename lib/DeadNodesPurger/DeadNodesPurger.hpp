#pragma once
#include <Observer.hpp>
#include <map>

class DeadNodesPurger : public Observable
{

private:
	class ListElement
	{

	public:
		unsigned long mLastKeepAlive;
		uint8_t mTries;
	
		inline ListElement(unsigned long lastKeepAlive)	:mLastKeepAlive(lastKeepAlive), mTries(0)	{}
	};

private:
	std::map<uint16_t, ListElement*> mConnectedNodes;
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