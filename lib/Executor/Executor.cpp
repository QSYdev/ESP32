#include <Executor.hpp>
#include <Arduino.h>

uint16_t Executor::STEP_INDEX = 0;

Executor::BiMap::BiMap(std::list<uint16_t>& associationList)
	:mPhysicalNodes(new uint16_t[associationList.size() + 1])
{
	int i = 1;
	for (uint16_t& physicalId : associationList)
	{
		mPhysicalNodes[i] = physicalId;
		mLogicalNodes[physicalId] = i;
		i++;
	}
}

Executor::BiMap::~BiMap()
{
	delete[] mPhysicalNodes;
}

void Executor::PreInitTask::start()
{ 
	mStart = true;
	mDelayIndex = 0;
	mElapsedTime = millis(); 
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
			const ExecutionStarted event;
			mExecutor->notify(&event);
			if (mExecutor->hasNextStep())
			{
				mExecutor->mCurrentStep = mExecutor->getNextStep();
				mExecutor->prepareStep();
				mExecutor->mPreInitTask.stop();
				mExecutor->mRoutineTimeOutTask.start();
			}
			else
			{
				mExecutor->finalizeRoutine(false);
			}
		}
		mDelayIndex++;
	}
}

void Executor::RoutineTimeOutTask::start()
{ 
	mStart = true; 
	mElapsedTime = millis(); 
}

void Executor::RoutineTimeOutTask::tick()
{
	if (!mStart || !mRoutineTimeOut)
		return;

	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= mRoutineTimeOut)
		mExecutor->finalizeRoutine(true);
}

void Executor::StepTimeOutTask::start(unsigned long stepTimeOut, uint16_t stepIndex)
{ 
	mStart = true;
	mStepTimeOut = stepTimeOut;
	mStepIndex = stepIndex;
	mElapsedTime = millis();
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

		const ExecutionStepTimeOut event;
		mExecutor->notify(&event);
		
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
	}
}

Executor::Executor(std::list<uint16_t>& associationList, unsigned long routineTimeOut, void (*toucheEvent)(Executor*, uint16_t, uint16_t, const Color&, uint32_t), void (*stepTimeOutEvent)(Executor*, uint16_t))
	:mBiMap(associationList), mTouchedNodes(new bool[associationList.size() + 1]), mExpressionTree(nullptr), mPreInitTask(this), mRoutineTimeOutTask(this, routineTimeOut), mStepTimeOutTask(this), mToucheEvent(toucheEvent), mStepTimeOutEvent(stepTimeOutEvent), mExecutionFinished(false)
{
	memset(mTouchedNodes, 0, associationList.size() + 1);
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

void Executor::touche(uint16_t physicalId, uint16_t stepIndex, const Color& color, uint32_t delay)
{
	uint16_t logicalId = mBiMap.getLogicalId(physicalId);
	if (logicalId && stepIndex == mStepIndex)
	{
		if (mToucheEvent)
			mToucheEvent(this, physicalId, stepIndex, color, delay);

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
		
		const CommandRequest event({physicalId, configuration->mColor, delay, mStepIndex}, true);
		notify(&event);
	}

	mExpressionTree = new ExpressionTree(mCurrentStep->mExpression);

	if (mCurrentStep->mStepTimeOut)
		mStepTimeOutTask.start(mCurrentStep->mStepTimeOut + maxDelay, mStepIndex);
}

void Executor::turnAllNodes(const Color& color)
{
	for (uint16_t i = 0; i < mBiMap.size(); i++)
	{
		uint16_t physicalId = mBiMap.getPhysicalId(i + 1);
		const CommandRequest event({physicalId, color, 0, 0}, true);
		notify(&event);
	}
}

void Executor::finalizeStep()
{
	const Color noColor = {0, 0, 0};
	for (const NodeConfiguration* nodeConfiguration : mCurrentStep->mNodeConfiguration) 
	{
		if (!mTouchedNodes[nodeConfiguration->mLogicalId]) 
		{
			uint16_t physicalId = mBiMap.getPhysicalId(nodeConfiguration->mLogicalId);
			const CommandRequest event({physicalId, noColor, 0, 0}, true);
			notify(&event);
		}
	}

	memset(mTouchedNodes, 0, mBiMap.size() + 1);

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
	mExecutionFinished = true;
	if (notify)
	{
		const ExecutionFinished event;
		this->notify(&event);
	}
}