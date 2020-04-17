#include "App.h"

App::App(float targetFPS) : targetFPS(targetFPS)
{
	wnds.push_back(new DXWindow(400, 400, "MainWindow"));
	wnds.push_back(new DXWindow(200, 200, "1"));
	wnds.push_back(new DXWindow(200, 200, "2"));
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
			oss << "window exeted with code " << Window::OnWindowQuit(*ret) << '\n';
			OutputDebugString(oss.str().c_str());
		}
		else if ((elapsedTime = clock.Peek()) > 1 / targetFPS)
		{
			for(DXWindow* wnd : wnds) wnd->DoFrame(elapsedTime);
			clock.Set();
		}
	}
	return 0;
}