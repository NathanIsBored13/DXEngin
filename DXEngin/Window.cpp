#include "Window.h"

std::vector<Window::WindowStatus> Window::statuses;
std::vector<std::pair<int, Window*>> Window::activeWnds;
Window::WindowTemplate Window::WindowTemplate::wndClass;

Window::WindowTemplate::WindowTemplate() noexcept : hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wnd =
	{
		sizeof(wnd),
		CS_OWNDC,
		HandleMsgSetup,
		0,
		0,
		GetInstance(),
		static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 32, 32, 0)),
		nullptr,
		nullptr,
		nullptr,
		GetName(),
		static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 16, 16, 0))
	};
	RegisterClassEx(&wnd);
}

Window::WindowTemplate::~WindowTemplate()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowTemplate::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowTemplate::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* name, Window* parent) : width(width), height(height), name(name), index(statuses.size())
{
	statuses.push_back( {this, parent, true, false, -1} );
	activeWnds.push_back({ index, this });
	RECT wr
	{
		100,
		100,
		width + 100,
		height + 100
	};
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw EE_WINDOW_EXCEPTION(GetLastError());
	}
	hWnd = CreateWindow(WindowTemplate::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowTemplate::GetInstance(), this);
	if (hWnd == nullptr)
	{
		throw EE_WINDOW_EXCEPTION(GetLastError());
	}
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	gfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::DoFrames(float elapsed) noexcept
{
	for (std::pair<int, Window*> wnd : activeWnds)
	{
		if (!statuses[wnd.first].paused)
		{
			wnd.second->DoFrame(elapsed);
		}
	}
}

bool Window::IsActiveWindow() noexcept
{
	return activeWnds.size() > 0;
}

bool Window::IsWindowActive(Window* pWnd) noexcept
{
	bool ret = false;
	for (WindowStatus wnd : statuses)
	{
		if (wnd.wnd == pWnd)
		{
			ret = wnd.open;
		}
	}
	return ret;
}

void Window::TrimWindows(std::vector<Window*>* wnds) noexcept
{
	wnds->erase(std::remove_if(wnds->begin(), wnds->end(), [](Window* wnd) { return !IsWindowActive(wnd); }), wnds->end());
}

std::optional<int> Window::ProcessMessages() noexcept
{
	std::optional<int> ret;
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) && !ret)
	{
		if (msg.message == WM_QUIT)
		{
			statuses[msg.wParam].open = false;
			statuses[msg.wParam].wnd->~Window();
			ret = statuses[msg.wParam].exitCode;
			activeWnds.erase(std::remove_if(activeWnds.begin(), activeWnds.end(), [](std::pair<int, Window*> wnd) { return !statuses[wnd.first].open; }), activeWnds.end());
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return ret;
}

void Window::PostQuit(int exitCode) noexcept
{
	statuses[index].exitCode = exitCode;
	PostQuitMessage(index);
}

void Window::SetWindowTitle(const char* title) noexcept
{
	name = title;
	SetWindowText(hWnd, title);
}

const char* Window::GetWindowTitle() noexcept
{
	return name;
}

int Window::GetExitCode() noexcept
{
	return statuses[index].exitCode;
}

Graphics& Window::GetGFX()
{
	return *gfx;
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  noexcept
{
	LRESULT ret;
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		ret = pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	else
	{
		ret = DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return ret;
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  noexcept
{
	return reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
		case WM_CLOSE:
		{
			PostQuit(0);
			return 0;
		}
		case WM_KILLFOCUS:
		{
			kbd.ClearState();
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
			{
				kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_CHAR:
		{
			kbd.OnChar(static_cast<unsigned char>(wParam));
			break;
		}
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
			{
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow())
				{
					mouse.OnMouseEnter();
					SetCapture(hWnd);
				}
			}
			else
			{
				if (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON))
				{
					mouse.OnMouseMove(pt.x, pt.y);
				}
				else
				{
					mouse.OnMouseLeave();
					ReleaseCapture();
				}
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnMiddlePressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_MBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnMiddleReleased(pt.x, pt.y);
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse.OnWheelUp(pt.x, pt.y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				mouse.OnWheelDown(pt.x, pt.y);
			}
			const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouse.OnWheelDelta(pt.x, pt.y, delta);
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}