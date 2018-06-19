#pragma once
#include <Observer.hpp>

class Executor : public Observable
{

private:
	class PreInit
	{
	private:
		unsigned long mDelays[9] = {0, 500, 500, 500, 500, 150, 150, 150, 150};
		
		Executor* mExecutor;
		unsigned long mElapsedTime;
		uint8_t mDelayIndex;

	public:
		void init(Executor* executor);
		void tick();
	};

private:
	PreInit mPreInitTask;
	
public:
	Executor();

	void init();
	void tick();
};