#pragma once

#include <chrono>
class Timer
{
public:
	Timer();
	float Mark();
	float Peek() const;
	void Set();
private:
	std::chrono::steady_clock::time_point clock;
};

