#pragma once
#include <Executor.hpp>
#include <QSYPacket.hpp>

class PlayerExecutor : public Executor
{

private:
	std::list<const Color> mPlayersAndColors;
	const bool mWaitForAllPlayers;
	const unsigned long mStepTimeOut;
	const unsigned long mStepDelay;
	const uint16_t mStepsCount;
	const bool mStopOnStepTimeOut;
	const uint16_t mNodesCount;

	uint16_t mStepIndex;
	Step* mLastCreatedStep;
	
public:
	PlayerExecutor(std::list<uint16_t>& associationList, const std::list<const Color>& playersAndColors, bool waitForAllPlayers, unsigned long stepTimeOut, unsigned long stepDelay, uint16_t stepsCount, bool stopOnStepTimeOut, unsigned long executionTimeOut);
	~PlayerExecutor();

protected:
	bool hasNextStep() override;
	const Step* getNextStep() override;

private:
	static void toucheEvent(Executor* executor, uint16_t physicalId, uint16_t stepIndex, const Color& color, uint32_t delay);
	static void stepTimeOutEvent(Executor* executor, uint16_t stepIndex);

};