#include "WindowManager.h"

std::vector<Window*> WindowManager::wnds;

void WindowManager::RegisterWindow(Window* wnd) noexcept
{
	wnds.push_back(wnd);
}

void WindowManager::DoFrames(float elapsed) noexcept
{
	for (Window* wnd : wnds)
	{
		wnd->DoFrame(elapsed);
	}
}

bool WindowManager::IsActiveWindow() noexcept
{
	return wnds.size() > 0;
}

bool WindowManager::IsWindowActive(Window* pWnd) noexcept
{
	bool ret = false;
	for (Window* wnd : wnds)
	{
		if (wnd == pWnd)
		{
			ret = true;
		}
	}
	return ret;
}

std::optional<int> WindowManager::ProcessMessages() noexcept
{
	std::optional<int> ret;
	MSG msg;
	while (!ret && PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			OutputDebugString("Quit\n");
			reinterpret_cast<Window*>(msg.lParam)->~Window();
			for (size_t i = 0; i < wnds.size(); i++)
			{
				if (wnds[i] == reinterpret_cast<Window*>(msg.lParam))
				{
					wnds.erase(wnds.begin() + i);
				}
			}
			ret = msg.wParam;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return ret;
}