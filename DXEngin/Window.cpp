#include "Window.h"

Window::WindowTemplate Window::WindowTemplate::wndClass;

Window::WindowTemplate::WindowTemplate() noexcept : hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEXA wnd =
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

Window::Window(int width, int height, const char* name, Window* parent) : width(width), height(height), name(name), parent(parent)
{
	if (parent != nullptr)
	{
		parent->AddChild(this);
	}
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

Window::~Window() {}

void Window::AddChild(Window* child) noexcept
{
	children.push_back(child);
}

void Window::RemoveChild(Window* child) noexcept
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] == child)
		{
			children.erase(children.begin() + i);
		}
	}
}

void Window::PostQuit(int exitCode) noexcept
{
	for (Window* child : children)
	{
		child->PostQuit(1);
	}
	if (parent != nullptr && exitCode != 1)
	{
		parent->RemoveChild(this);
	}
	DestroyWindow(hWnd);
	PostMessage(nullptr, WM_QUIT, exitCode, reinterpret_cast<LONG_PTR>(this));
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