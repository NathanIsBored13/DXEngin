#include "DXWindow.h"

DXWindow::DXWindow(int width, int height, const char* name) : Window(width, height, name) {}

DXWindow::~DXWindow() {}

void DXWindow::DoFrame(float elapsedTime)
{
	static Timer timer;
	SetWindowTitle(std::to_string(1 / elapsedTime).append(" FPS").c_str());
	float s = static_cast<float>(sin(timer.Peek() / 2 + 0.5));
	GetGFX().ClearBuffer(s, s, 1);
	GetGFX().EndFrame();
}