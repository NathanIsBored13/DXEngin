#pragma once

#include "DXWindow.h"
#include "Timer.h"

#include <ostream>
#include <vector>

class App
{
public:
	App(float);
	int Begin();
private:
	std::vector<Window*> wnds;
	float targetFPS;
};