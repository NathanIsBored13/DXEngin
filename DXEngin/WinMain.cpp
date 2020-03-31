#include "WinMain.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		Window* window1 = new Window(400, 400, "MainWindow1");
		Window* window2 = new Window(300, 300, "MainWindow2");
		Window* window3 = new Window(200, 200, "MainWindow3");

		MSG msg;
		BOOL gResult;
		do
		{
			if ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (gResult == 0)
			{
				std::ostringstream oss;
				oss << "window exited with code " << Window::DeconstructWindow(msg.wParam) << "\n";
				OutputDebugString(oss.str().c_str());
			}
			else
			{
				throw EE_MSG_LOOP_EXCEPTION_GLE();
			}
		} while (Window::AreActiveWindows());
		return 0;
	}
	catch (const ExtendedException& e)
	{
		MessageBox(nullptr, e.what(), "Extended exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (std::exception & e)
	{
		MessageBox(nullptr, e.what(), "Standerd exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}