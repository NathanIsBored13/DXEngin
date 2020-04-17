#include "App.h"

App::App(float targetFPS) : targetFPS(targetFPS) 
{
	wnds.push_back(new Window(400, 400, "1"));
	wnds.push_back(new Window(400, 400, "2"));
	wnds.push_back(new Window(400, 400, "3"));
}

int App::Begin()
{

	Timer clock;
	std::optional<int> ret;
	float elapsedTime;
	while (Window::IsActiveWindow())
	{
		if (std::optional<int> ret = Window::ProcessMessages())
		{
			std::ostringstream oss;
			oss << "window exeted with code " << *ret << '\n';
			OutputDebugString(oss.str().c_str());
		}
		else if ((elapsedTime = clock.Peek()) > 1 / targetFPS)
		{
			RunFrames(elapsedTime);
			clock.Set();
		}
	}
	return *ret;
}

void App::RunFrames(float elapsedTime)
{
	static Timer timer;
	for (Window* wnd : wnds)
	{
		wnd->SetWindowTitle(std::to_string(1 / elapsedTime).append(" FPS").c_str());
		wnd->GetGFX().ClearBuffer(sin(timer.Peek()), sin(timer.Peek()), 1);
		wnd->GetGFX().EndFrame();
	}
}