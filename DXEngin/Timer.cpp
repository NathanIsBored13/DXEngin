#include "Timer.h"

Timer::Timer()
{
	clock = std::chrono::steady_clock::now();
}

float Timer::Mark()
{
	const std::chrono::steady_clock::time_point old = clock;
	clock = std::chrono::steady_clock::now();
	return std::chrono::duration<float>(clock - old).count();
}

float Timer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - clock).count();
}

void Timer::Set()
{
	clock = std::chrono::steady_clock::now();
}
