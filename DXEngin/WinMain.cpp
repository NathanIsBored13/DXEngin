#include "WinMain.h"
#include "Window.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	const char *pClassName = "MainWindow";
	Window* window1 = new Window(200, 200, "MainWindow1");
	Window* window2 = new Window(300, 300, "MainWindow2");
	Window* window3 = new Window(400, 400, "MainWindow3");
	MSG msg;
	BOOL gResult;
	do
	{
		if ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (Window::GetWindowCount() > 0);
	return gResult == -1 ? -1 : msg.wParam;
}