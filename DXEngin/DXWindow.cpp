#include "DXWindow.h"

DXWindow::DXWindow(int width, int height, const char* name, Window* parent) : Window(width, height, name, parent)
{
	if (parent == nullptr)
	{
		WindowManager::RegisterWindow(new R(200, 200, "MainWindow", this));
		WindowManager::RegisterWindow(new G(200, 200, "MainWindow", this));
		WindowManager::RegisterWindow(new B(200, 200, "MainWindow", this));
	}
}

DXWindow::~DXWindow() {}

void DXWindow::DoFrame(float elapsedTime)
{
	static Timer timer;
	SetWindowTitle(std::to_string(1 / elapsedTime).append(" FPS").c_str());
	float s = static_cast<float>(timer.Peek() - std::floor(timer.Peek()));
	GetGFX().ClearBuffer(s, s, s);
	GetGFX().EndFrame();
}