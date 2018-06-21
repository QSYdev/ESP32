#include <Executor.hpp>

uint16_t Executor::STEP_INDEX = 0;

Executor::BiMap::BiMap(std::list<uint16_t>& associationList)
	:mPhysicalNodes(new uint16_t[associationList.size() + 1])
{
	int i = 1;
	for (uint16_t& physicalId : associationList)
	{
		mPhysicalNodes[i] = physicalId;
		mLogicalNodes[physicalId] = i;
	}
}

Executor::BiMap::~BiMap()
{
	delete[] mPhysicalNodes;
}

void Executor::PreInitTask::tick()
{
	if (!mStart || mDelayIndex > 8)
		return;

	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= mDelays[mDelayIndex])
	{
		if (mDelayIndex < 8)
		{
			mElapsedTime = timeNow;
			const Color col((mDelayIndex == 0 || mDelayIndex == 2) ? 0xF : 0, (mDelayIndex == 4 || mDelayIndex == 6) ? 0xF : 0, 0);
			mExecutor->turnAllNodes(col);
		}
		else
		{
			mExecutor->turnAllNodes({0, 0, 0});
			mExecutor->mCurrentStep = mExecutor->getNextStep();
			mExecutor->prepareStep();
			mExecutor->mPreInitTask.stop();
			mExecutor->mRoutineTimeOutTask.start();
			Serial.println("Arranca la rutina");
		}
		mDelayIndex++;
	}
}

void Executor::RoutineTimeOutTask::tick()
{
	if (!mStart || !mRoutineTimeOut)
		return;

	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= mRoutineTimeOut)
	{
		mExecutor->finalizeRoutine(true);
		Serial.println("RoutineTimeOut");
	}
}

void Executor::StepTimeOutTask::tick()
{
	if (!mStart || !mStepTimeOut)
		return;
	
	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= mStepTimeOut)
	{
		if (mExecutor->mStepTimeOutEvent)
			mExecutor->mStepTimeOutEvent(mExecutor, mStepIndex);

		mExecutor->finalizeStep();
		if (mExecutor->hasNextStep() && !mExecutor->mCurrentStep->mStopOnTimeOut)
		{
			mExecutor->mCurrentStep = mExecutor->getNextStep();
			mExecutor->prepareStep();
		}
		else 
		{
			mExecutor->finalizeRoutine(true);
		}
		Serial.println("StepTimeOut");
	}
}

Executor::Executor(std::list<uint16_t>& associationList, unsigned long routineTimeOut, void (*toucheEvent)(Executor*, uint16_t, uint16_t, Color&, uint32_t), void (*stepTimeOutEvent)(Executor*, uint16_t))
	:mBiMap(associationList), mTouchedNodes(new bool[associationList.size() + 1]), mPreInitTask(this), mRoutineTimeOutTask(this, routineTimeOut), mStepTimeOutTask(this), mToucheEvent(toucheEvent), mStepTimeOutEvent(stepTimeOutEvent)
{
}

Executor::~Executor()
{
	finalizeRoutine(false);
}

void Executor::init()
{
	mPreInitTask.start();
}

void Executor::tick()
{
	mPreInitTask.tick();
	mRoutineTimeOutTask.tick();
	mStepTimeOutTask.tick();
}

void Executor::touche(uint16_t physicalId, uint16_t stepIndex, Color& col, uint32_t delay)
{
	uint16_t logicalId = mBiMap.getLogicalId(physicalId);
	if (logicalId && stepIndex == mStepIndex)
	{
		if (mToucheEvent)
			mToucheEvent(this, physicalId, stepIndex, col, delay);

		mTouchedNodes[logicalId] = true;
		if (mExpressionTree->evaluateExpression(mTouchedNodes))
		{
			finalizeStep();
			if (hasNextStep())
			{
				mCurrentStep = getNextStep();
				prepareStep();
			}
			else
			{
				finalizeRoutine(true);
			}
		}
	}
}

void Executor::prepareStep()
{
	mStepIndex = (++STEP_INDEX) ? STEP_INDEX : 1;

	uint32_t maxDelay = 0;
	for (const NodeConfiguration* configuration : mCurrentStep->mNodeConfiguration)
	{
		uint16_t physicalId = mBiMap.getPhysicalId(configuration->mLogicalId);
		uint32_t delay = configuration->mDelay;
		if (delay > maxDelay)
			maxDelay = delay;
		
		const CommandRequest event(physicalId, configuration->mColor, delay, mStepIndex);
		notify(&event);
	}

	mExpressionTree = new ExpressionTree(mCurrentStep->mExpression);

	if (mCurrentStep->mStepTimeOut > 0)
		mStepTimeOutTask.start(mCurrentStep->mStepTimeOut + maxDelay, mStepIndex);
}

void Executor::turnAllNodes(const Color& col)
{
	for (uint16_t i = 1; i < mBiMap.size(); i++)
	{
		uint16_t physicalId = mBiMap.getPhysicalId(i);
		const CommandRequest event(physicalId, col, 0, 0);
		notify(&event);
	}
}

void Executor::finalizeStep()
{
	Color noColor = {0, 0, 0};
	for (const NodeConfiguration* nodeConfiguration : mCurrentStep->mNodeConfiguration) 
	{
		if (!mTouchedNodes[nodeConfiguration->mLogicalId]) 
		{
			uint16_t physicalId = mBiMap.getPhysicalId(nodeConfiguration->mLogicalId);
			const CommandRequest event(physicalId, noColor, 0, 0);
			notify(&event);
		}
	}

	for (int i = 0; i < mBiMap.size(); i++)
		mTouchedNodes[i] = false;

	mStepTimeOutTask.stop();
	delete mExpressionTree;
	mExpressionTree = nullptr;
}

void Executor::finalizeRoutine(bool notify)
{
	turnAllNodes({0, 0, 0});
	delete mExpressionTree;
	mExpressionTree = nullptr;
	delete[] mTouchedNodes;
	mTouchedNodes = nullptr;
	mPreInitTask.stop();
	mRoutineTimeOutTask.stop();
	mStepTimeOutTask.stop();
	//TODO notify.
	Serial.println("Rutina Finalizada.");
}