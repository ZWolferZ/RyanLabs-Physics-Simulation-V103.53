#pragma once

#pragma region Includes
// Include{s}
#include <chrono>
#pragma endregion

using namespace std::chrono;

/// <summary>
/// Represents a timer for measuring time intervals.
/// </summary>
class Timer
{
public:
#pragma region Constructor
	/// <summary>
	/// Initializes a new instance of the Timer class.
	/// </summary>
	Timer();
#pragma endregion

#pragma region Time Methods
	/// <summary>
	/// Gets the delta time since the last frame.
	/// </summary>
	/// <returns>The delta time in seconds.</returns>
	float GetDeltaTime() const;

	/// <summary>
	/// Updates the timer to the current time.
	/// </summary>
	void Tick();
#pragma endregion

private:
#pragma region Member Variables
	steady_clock::time_point lastFrame;
#pragma endregion
};