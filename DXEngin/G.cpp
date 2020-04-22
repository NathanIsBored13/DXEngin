#include "G.h"

G::G(int width, int height, const char* name) : Window(width, height, name, nullptr) {}

G::~G() {}

void G::DoFrame(float elapsedTime)
{
	static Timer timer;
	SetWindowTitle(std::to_string(1 / elapsedTime).append(" FPS").c_str());
	float s = static_cast<float>(timer.Peek() - std::floor(timer.Peek()));
	GetGFX().ClearBuffer(0, s, 0);
	GetGFX().EndFrame();
}
