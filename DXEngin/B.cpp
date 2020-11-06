#include "B.h"

B::B(int width, int height, const char* name, Window* parent) : Window(width, height, name, parent) {}

B::~B() {}

void B::DoFrame(float elapsedTime)
{
	static Timer timer;
	SetWindowTitle(std::to_string(1 / elapsedTime).append(" FPS").c_str());
	float s = static_cast<float>(timer.Peek() - std::floor(timer.Peek()));
	GetGFX().ClearBuffer(0, 0, s);
	GetGFX().EndFrame();
}
