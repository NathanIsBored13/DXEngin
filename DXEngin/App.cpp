#include "App.h"

App::App(float targetFPS) : targetFPS(targetFPS)
{
}

int App::Begin()
{
	new DXWindow(400, 400, "MainWindow");
	new R(200, 200, "1");
	new G(200, 200, "2");
	new B(200, 200, "3");

	Timer clock;
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
			clock.Set();
			Window::DoFrames(elapsedTime);
		}
	}
	return 0;
}