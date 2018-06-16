#include <DeadNodesPurger.hpp>
#include <Arduino.h>

DeadNodesPurger::DeadNodesPurger()
	:mConnectedNodes(), mElapsedTime(millis())
{

}

void DeadNodesPurger::init()
{

}

void DeadNodesPurger::tick()
{
	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= QSY_DEAD_NODES_PURGER_PERIOD)
	{
		Serial.print("SIZE DNP = ");
		Serial.println(mConnectedNodes.size());

		mElapsedTime = timeNow;
		List<uint16_t> purgedNodes;
		mConnectedNodes.begin();
		while (!mConnectedNodes.end())
		{
			ListElement* elem = mConnectedNodes.next();
			if (timeNow - elem->mLastKeepAlive > QSY_MAX_ALLOWED_TIME)
			{
				++elem->mTries;
				if (elem->mTries >= QSY_MAX_TRIES)
					purgedNodes.add(elem->mPhysicalId);
			}
			else
			{
				elem->mTries = 0;
			}
		}

		purgedNodes.begin();
		while (!purgedNodes.end())
		{
			DisconnectedNode event(purgedNodes.next());
			notify(&event);
		}
	}
}

void DeadNodesPurger::hello(uint16_t physicalId)
{
	mConnectedNodes.addById(physicalId, new ListElement(physicalId, millis()));
}

void DeadNodesPurger::touche(uint16_t physicalId)
{
	keepAlive(physicalId);
}

void DeadNodesPurger::keepAlive(uint16_t physicalId)
{
	ListElement* element = mConnectedNodes.findById(physicalId);
	element->mLastKeepAlive = millis();
}

void DeadNodesPurger::disconnectedNode(uint16_t physicalId)
{
	delete mConnectedNodes.removeById(physicalId);
}