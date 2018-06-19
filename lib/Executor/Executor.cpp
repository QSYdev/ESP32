#include <Executor.hpp>
#include <Arduino.h>

Executor::Executor()
{

}

void Executor::PreInit::init(Executor* executor)
{
	mExecutor = executor;
	mElapsedTime = millis();
	mColorIndex = 0;
	mDelayIndex = 0;
}

void Executor::PreInit::tick()
{
	if (mDelayIndex == 2)
		return;

	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= mDelays[mDelayIndex])
	{
		mElapsedTime = timeNow;
		const CommandRequest event(19, mColors[mColorIndex], 0, 0);
		mExecutor->notify(&event);
		mColorIndex = (mColorIndex + 1) % 2;
		mDelayIndex = (!mColorIndex) ? mDelayIndex + 1 : mDelayIndex;
	}
}

void Executor::init()
{
	mPreInitTask.init(this);
}

void Executor::tick()
{
	mPreInitTask.tick();
}