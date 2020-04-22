#pragma once

#include "Window.h"
#include "Timer.h"

class G : public Window
{
public:
	G(int, int, const char*);
	~G();
	void DoFrame(float) override;
private:
};

