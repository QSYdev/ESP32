#include <Executor.hpp>

Executor::Executor(unsigned long timeOut)
	:mPreInitTask(this), mTimeOutTask(this, timeOut)
{
}

void Executor::PreInitTask::tick()
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
			mExecutor->mTimeOutTask.start();
			Serial.println("Arranca la rutina");
		}
		mDelayIndex++;
	}
}

void Executor::TimeOutTask::tick()
{
	if (!mStart || !mTimeOut)
		return;

	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= mTimeOut)
	{
		// TODO TimeOut.
		Serial.print("TIME OUT AFTER ");
		Serial.print(timeNow - mElapsedTime);
		Serial.println(" SECONDS");
		mStart = false;
	}
}

void Executor::init()
{
	mPreInitTask.start();
}

void Executor::tick()
{
	mPreInitTask.tick();
	mTimeOutTask.tick();
}