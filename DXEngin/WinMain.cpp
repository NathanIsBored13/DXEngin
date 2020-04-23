#include "WinMain.h"

#define TarfetFPS 60

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		WindowManager::RegisterWindow(new DXWindow(400, 400, "MainWindow", nullptr));

		Timer clock;
		while (WindowManager::IsActiveWindow())
		{
			if (std::optional<int> ret = WindowManager::ProcessMessages())
			{
				std::ostringstream oss;
				oss << "\n-----\nwindow exited with code " << *ret << "\n-----\n\n";
				OutputDebugString(oss.str().c_str());
			}
			else if (float elapsedTime = clock.Peek(); elapsedTime > 1 / TarfetFPS)
			{
				clock.Set();
				WindowManager::DoFrames(elapsedTime);
			}
		}
	}
	catch (const ExtendedException& e)
	{
		MessageBox(nullptr, e.what(), "Extended exception", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
	catch (std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standerd exception", MB_OK | MB_ICONEXCLAMATION);
		return -2;
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
		return -3;
	}
	return 0;
}