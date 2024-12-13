#include "Timer.h"

Timer::Timer()
{
	lastFrame = steady_clock::now();
}

float Timer::GetDeltaTime() const
{
	return duration<float>(steady_clock::now() - lastFrame).count();
}

void Timer::Tick()
{
	lastFrame = steady_clock::now();
}
