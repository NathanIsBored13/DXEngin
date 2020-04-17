#pragma once

#include "Window.h"
#include "Timer.h"

class DXWindow : public Window
{
public:
	DXWindow(int, int, const char*);
	~DXWindow();
	void DoFrame(float) override;
private:
};

