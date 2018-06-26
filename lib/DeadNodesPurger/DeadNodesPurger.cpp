#include <DeadNodesPurger.hpp>
#include <Arduino.h>

DeadNodesPurger::DeadNodesPurger()
	:mConnectedNodes(), mElapsedTime(0)
{
}

void DeadNodesPurger::init()
{
	mElapsedTime = millis();
}

void DeadNodesPurger::tick()
{
	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= QSY_WIFI_DEAD_NODES_PURGER_PERIOD)
	{
		mElapsedTime = timeNow;
		std::list<uint16_t> purgedNodes;

		std::map<uint16_t, ListElement*>::iterator iterator = mConnectedNodes.begin();
		while (iterator != mConnectedNodes.end())
		{
			ListElement* elem = iterator->second;
			if (timeNow - elem->mLastKeepAlive > QSY_WIFI_MAX_ALLOWED_TIME)
			{
				++elem->mTries;
				if (elem->mTries >= QSY_WIFI_MAX_TRIES)
					purgedNodes.push_back(iterator->first);
			}
			else
			{
				elem->mTries = 0;
			}
			++iterator;
		}

		for (uint16_t physicalId : purgedNodes)
		{
			DisconnectedNode event(physicalId);
			notify(&event);
		}
	}
}

void DeadNodesPurger::hello(uint16_t physicalId)
{
	mConnectedNodes[physicalId] = new ListElement(millis());
}

void DeadNodesPurger::touche(uint16_t physicalId)
{
	keepAlive(physicalId);
}

void DeadNodesPurger::keepAlive(uint16_t physicalId)
{
	mConnectedNodes[physicalId]->mLastKeepAlive = millis();
}

void DeadNodesPurger::disconnectedNode(uint16_t physicalId)
{
	ListElement* element = mConnectedNodes[physicalId];
	mConnectedNodes.erase(physicalId);
	delete element;
}