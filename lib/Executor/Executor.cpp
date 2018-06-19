#include <Executor.hpp>
#include <Arduino.h>

Executor::Executor()
	:mElapsedTime(millis()), index(0)
{

}

void Executor::init()
{
	
}

void Executor::tick()
{
	unsigned long timeNow = millis();
	if (timeNow - mElapsedTime >= 500)
	{
		mElapsedTime = timeNow;

		struct color col;
		col.blue = 0;
		col.green = 0;
		col.red = (index) ? 0xF : 0x0;
		index = (index + 1) % 2;

		const CommandRequest event(19, col, 0, 0);
		notify(&event);
	}
}