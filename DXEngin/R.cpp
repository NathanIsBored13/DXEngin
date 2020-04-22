#include "R.h"

R::R(int width, int height, const char* name) : Window(width, height, name, nullptr) {}

R::~R() {}

void R::DoFrame(float elapsedTime)
{
	static Timer timer;
	SetWindowTitle(std::to_string(1 / elapsedTime).append(" FPS").c_str());
	float s = static_cast<float>(timer.Peek() - std::floor(timer.Peek()));
	GetGFX().ClearBuffer(s, 0, 0);
	GetGFX().EndFrame();
}
