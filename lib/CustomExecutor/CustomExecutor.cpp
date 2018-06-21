#include <CustomExecutor.hpp>

CustomExecutor::CustomExecutor(const Routine* routine, std::list<uint16_t>& associationList)
	:Executor(associationList, routine->mRoutineTimeOut, &toucheEvent, &stepTimeOutEvent), mRoutine(routine), mIterator(routine->mSteps.begin())
{
}

CustomExecutor::~CustomExecutor()
{
	delete mRoutine;
}

bool CustomExecutor::hasNextStep()
{
	return mIterator != mRoutine->mSteps.end();
}

const Step* CustomExecutor::getNextStep()
{
	const Step* currentStep = *mIterator;
	mIterator++;
	return currentStep;
}

void CustomExecutor::toucheEvent(Executor* executor, uint16_t physicalId, uint16_t stepIndex, Color& col, uint32_t delay)
{

}

void CustomExecutor::stepTimeOutEvent(Executor* executor, uint16_t stepIndex)
{

}