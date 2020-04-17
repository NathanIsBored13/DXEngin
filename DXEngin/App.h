#pragma once

#include "Window.h"
#include "Timer.h"

#include <ostream>
#include <vector>

class App
{
public:
	App(float targetFPS);
	int Begin();
private:
	void RunFrames(float elapsedTime);
	std::vector<Window*> wnds;
	float targetFPS;
};