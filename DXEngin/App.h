#pragma once

#include "Window.h"

#include <ostream>
#include <vector>

class App
{
public:
	App();
	int Begin();
private:
	void DoFrame();
	Window wnd;
};