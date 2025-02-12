#pragma region Includes
// Include{s}
#include "Timer.h"
#pragma endregion

#pragma region Constructor
Timer::Timer()
{
	lastFrame = steady_clock::now();
}
#pragma endregion

#pragma region Time Methods

// I mean this is pretty easy to understand.

float Timer::GetDeltaTime() const
{
	return duration<float>(steady_clock::now() - lastFrame).count();
}

void Timer::Tick()
{
	lastFrame = steady_clock::now();
}
#pragma endregion