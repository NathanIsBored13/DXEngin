#pragma once

#include "WindowManager.h"
#include "Window.h"
#include "Timer.h"
#include "R.h"

class DXWindow : public Window
{
public:
	DXWindow(int, int, const char*, Window*);
	~DXWindow();
	void DoFrame(float) override;
private:
};

