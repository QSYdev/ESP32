#include <PlayerExecutor.hpp>
#include <sstream>

PlayerExecutor::PlayerExecutor(std::list<uint16_t>& associationList, std::list<Color>& playersAndColors, bool waitForAllPlayers, unsigned long stepTimeOut, unsigned long stepDelay, uint16_t stepsCount, bool stopOnStepTimeOut, unsigned long executionTimeOut)
	:Executor(associationList, executionTimeOut, &toucheEvent, &stepTimeOutEvent), mPlayersAndColors(playersAndColors), mWaitForAllPlayers(waitForAllPlayers), mStepTimeOut(stepTimeOut), mStepDelay(stepDelay), mStepsCount(stepsCount), mStopOnStepTimeOut(stopOnStepTimeOut), mNodesCount(associationList.size()), mStepIndex(0), mLastCreatedStep(nullptr)
{
	srand(time(NULL));
}

PlayerExecutor::~PlayerExecutor()
{
	delete mLastCreatedStep;
	mLastCreatedStep = nullptr;
}

bool PlayerExecutor::hasNextStep()
{
	return mStepsCount == 0 || mStepIndex < mStepsCount;
}

uint16_t removeAtIndex(std::list<uint16_t>& list, int index)
{
	uint16_t result = 0;
	std::list<uint16_t>::iterator it = list.begin();

	for (uint16_t i = 0; i < index && it != list.end(); i++)
		++it;

	result = *it;
	list.erase(it);

	return result;
}

const Step* PlayerExecutor::getNextStep()
{
	delete mLastCreatedStep;
	mLastCreatedStep = nullptr;

	const char booleanOperator = (mWaitForAllPlayers) ? '&' : '|';
	std::list<uint16_t> usedIds;

	for (int i = 1; i <= mNodesCount; i++)
		usedIds.push_back(i);

	std::list<const NodeConfiguration*> nodesConfiguration;
	std::stringstream ss;

	for (const Color& color : mPlayersAndColors)
	{
		int logicalId = removeAtIndex(usedIds, rand() % usedIds.size());
		nodesConfiguration.push_back(new NodeConfiguration(logicalId, color, mStepDelay));
		ss << (int) logicalId << booleanOperator;
	}

	const char* expression = ss.str().substr(0, ss.str().size() - 1).data();

	++mStepIndex;
	return mLastCreatedStep = new Step(expression, mStepTimeOut, mStopOnStepTimeOut, nodesConfiguration);
}

void PlayerExecutor::toucheEvent(Executor* executor, uint16_t physicalId, uint16_t stepIndex, const Color& color, uint32_t delay)
{

}

void PlayerExecutor::stepTimeOutEvent(Executor* executor, uint16_t stepIndex)
{

}