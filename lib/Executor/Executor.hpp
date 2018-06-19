#pragma once
#include <Observer.hpp>

class Executor : public Observable
{

private:
	unsigned long mElapsedTime;
	int index;
	
public:
	Executor();

	void init();
	void tick();
};