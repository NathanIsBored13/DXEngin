#pragma once

#include "Window.h"
#include "Timer.h"

class R : public Window
{
public:
	R(int, int, const char*);
	~R();
	void DoFrame(float) override;
private:
};

