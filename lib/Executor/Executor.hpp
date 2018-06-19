#pragma once
#include <Observer.hpp>

class Executor : public Observable
{

private:
	class PreInit
	{
	private:
		struct color mColors[2] = { {0xF, 0x0, 0x0}, {0x0, 0xF, 0x0} };
		unsigned long mDelays[2] = {500, 150};
		
		Executor* mExecutor;
		unsigned long mElapsedTime;
		uint8_t mColorIndex, mDelayIndex;

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