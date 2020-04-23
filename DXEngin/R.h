#pragma once

#include "Window.h"
#include "Timer.h"
#include "G.h"

class R : public Window
{
public:
	R(int, int, const char*, Window*);
	~R();
	void DoFrame(float) override;
private:
};

