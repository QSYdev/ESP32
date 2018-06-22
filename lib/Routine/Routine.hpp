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

	inline NodeConfiguration(uint16_t logicalId, const Color& color, uint32_t delay)	:mLogicalId(logicalId), mColor(color), mDelay(delay)	{}

};

class Step
{

public:
	const std::string mExpression;
	const unsigned long mStepTimeOut;
	const bool mStopOnTimeOut;
	const std::list<const NodeConfiguration*> mNodeConfiguration;

	inline Step(std::string& expression, unsigned long stepTimeOut, bool stopOnTimeOut, std::list<const NodeConfiguration*>& nodeConfiguration)	:mExpression(expression), mStepTimeOut(stepTimeOut), mStopOnTimeOut(stopOnTimeOut), mNodeConfiguration(nodeConfiguration)	{}
};

class Routine
{

public:
	const uint8_t mPlayersCount;
	const uint16_t mNodesCount;
	const unsigned long mRoutineTimeOut;
	const std::string mName;
	const std::list<const Step*> mSteps;

	inline Routine(uint8_t playersCount, uint16_t nodesCount, unsigned long routineTimeOut, std::string& name, std::list<const Step*>& steps)	:mPlayersCount(playersCount), mNodesCount(nodesCount), mRoutineTimeOut(routineTimeOut), mName(name), mSteps(steps)	{}
};