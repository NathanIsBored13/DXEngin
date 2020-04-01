#include "Window.h"

Window::WindowTemplate Window::WindowTemplate::wndClass;
std::vector<Window*> Window::windows;
int Window::windowCount;

Window::WindowTemplate::WindowTemplate() noexcept : hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wnd =
	{
		.cbSize = sizeof(wnd),
		.style = CS_OWNDC,
		.lpfnWndProc = HandleMsgSetup,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = GetInstance(),
		.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 32, 32, 0)),
		.hCursor = nullptr,
		.hbrBackground = nullptr,
		.lpszMenuName = nullptr,
		.lpszClassName = GetName(),
		.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 16, 16, 0))
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

Window::Window(int width, int height, const char* name) : width(width), height(height), name(name), index(Window::windows.size()), exitCode(-1)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
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
	Window::windowCount++;
	Window::windows.push_back(this);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

bool Window::AreActiveWindows() noexcept
{
	return windowCount > 0;
}

int Window::DeconstructWindow(int index) noexcept
{
	int ret = Window::windows[index]->exitCode;
	Window::windows[index]->~Window();
	Window::windowCount--;
	return ret;
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
	std::optional<LRESULT> ret = std::nullopt;
	std::ostringstream oss;
	switch (msg)
	{
	case WM_CLOSE:
		oss << "closed " << name << '\n';
		OutputDebugString(oss.str().c_str());
		exitCode = 0;
		PostQuitMessage(index);
		ret = 0;
		break;
	case WM_KEYDOWN:
		break;
	case WM_CHAR:
		oss << name << ": " << static_cast<char>(wParam) << '\n';
		OutputDebugString(oss.str().c_str());
		break;
	case WM_LBUTTONDOWN:
		POINTS pt = MAKEPOINTS(lParam);
		oss << name << ": (" << pt.x << ", " << pt.y << ")\n";
		OutputDebugString(oss.str().c_str());
		break;
	}
	return ret ? *ret : DefWindowProc(hWnd, msg, wParam, lParam);
}