#include "App.h"

App::App() : wnd(400, 400, "MainWindow") {}

int App::Begin()
{
	std::optional<int> ret;
	while (!(ret = Window::ProcessMessages()))
	{
		DoFrame();
	}
	return *ret;
}

void App::DoFrame()
{

}