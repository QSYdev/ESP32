#include <Executor.hpp>
#include <Arduino.h>

Executor::Executor()
{

}

void Executor::PreInit::init(Executor* executor)
{
	mExecutor = executor;
	mElapsedTime = millis();
	mDelayIndex = 0;
}

void Executor::PreInit::tick()
{
	if (mDelayIndex > 8)
		return;

	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= mDelays[mDelayIndex])
	{
		if (mDelayIndex < 8)
		{
			mElapsedTime = timeNow;
			color col((mDelayIndex == 0 || mDelayIndex == 2) ? 0xF : 0, (mDelayIndex == 4 || mDelayIndex == 6) ? 0xF : 0, 0);
			const CommandRequest event(19, col, 0, 0);
			mExecutor->notify(&event);
		}
		else
		{
			// TODO Arranca la rutina.
			Serial.println("Arranca la rutina");
		}
		mDelayIndex++;
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