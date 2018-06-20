#pragma once
#include <Observer.hpp>
#include <Arduino.h>

class Executor : public Observable
{

private:
	class PreInitTask
	{
	private:
		Executor* mExecutor;
		const unsigned long mDelays[9] = {0, 500, 500, 500, 500, 150, 150, 150, 150};
		unsigned long mElapsedTime;
		uint8_t mDelayIndex;

	public:
		inline PreInitTask(Executor* executor) 	:mExecutor(executor)	{}

		inline void start()		{ mDelayIndex = 0; mElapsedTime = millis(); }
		void tick();
	};

	class TimeOutTask
	{

	private:
		Executor* mExecutor;
		const unsigned long mTimeOut;
		unsigned long mElapsedTime;
		bool mStart;

	public:
		inline TimeOutTask(Executor* executor, unsigned long timeOut)	:mExecutor(executor), mTimeOut(timeOut), mStart(false)	{}

		inline void start()	{ mStart = true; mElapsedTime = millis(); }
		void tick();
	};

private:
	PreInitTask mPreInitTask;
	TimeOutTask mTimeOutTask;
	
public:
	Executor(unsigned long timeOut);

	void init();
	void tick();
};