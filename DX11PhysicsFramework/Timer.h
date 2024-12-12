#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
public:
	Timer();

	float GetDeltaTime() const;
	void Tick();

private:

	steady_clock::time_point lastFrame;
};
