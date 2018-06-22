#pragma once
#include <list>
#include <string>
#include <QSYPacket.hpp>

class NodeConfiguration
{

public:
	const uint16_t mLogicalId;
	const Color mColor;
	const uint32_t mDelay;

	inline NodeConfiguration(uint16_t logicalId, const Color& color, uint32_t delay)	
		:mLogicalId(logicalId), mColor(color), mDelay(delay)
	{
	}

};

class Step
{

public:
	const char* mExpression;
	const unsigned long mStepTimeOut;
	const bool mStopOnTimeOut;
	const std::list<const NodeConfiguration*> mNodeConfiguration;

	inline Step(const char* expression, unsigned long stepTimeOut, bool stopOnTimeOut, std::list<const NodeConfiguration*>& nodeConfiguration)	
		:mExpression(reinterpret_cast<const char*>(memcpy(new char[strlen(expression) + 1], expression, strlen(expression) + 1))), mStepTimeOut(stepTimeOut), mStopOnTimeOut(stopOnTimeOut), mNodeConfiguration(nodeConfiguration)
	{
	}
	
	inline ~Step()
	{
		delete[] mExpression;
		for (const NodeConfiguration* config : mNodeConfiguration)
			delete config;
	}
};

class Routine
{

public:
	const uint8_t mPlayersCount;
	const uint16_t mNodesCount;
	const unsigned long mRoutineTimeOut;
	const char* mName;
	const std::list<const Step*> mSteps;

	inline Routine(uint8_t playersCount, uint16_t nodesCount, unsigned long routineTimeOut, const char* name, std::list<const Step*>& steps)	
		:mPlayersCount(playersCount), mNodesCount(nodesCount), mRoutineTimeOut(routineTimeOut), mName(reinterpret_cast<const char*>(memcpy(new char[strlen(name) + 1], name, strlen(name) + 1))), mSteps(steps)
	{
	}

	inline ~Routine()
	{
		delete[] mName;
		for (const Step* step : mSteps)
			delete step;
	}
};