#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer
{
public:
	// disable copy/move constructors
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	void Start();
	void UpdateFrameTicks();
	float GetDeltaTime() const;
	unsigned int GetSleepTime(const unsigned int fps_);
	float GetCurrentTicks() const;

private:
	unsigned int prevTicks, currentTicks;
};
#endif // !TIMER_H