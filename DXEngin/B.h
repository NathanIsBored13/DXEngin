#pragma once

#include "Window.h"
#include "Timer.h"

class B : public Window
{
public:
	B(int, int, const char*, Window*);
	~B();
	void DoFrame(float) override;
private:
};