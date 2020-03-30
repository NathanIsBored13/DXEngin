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
		.hIcon = nullptr,
		.hCursor = nullptr,
		.hbrBackground = nullptr,
		.lpszMenuName = nullptr,
		.lpszClassName = GetName(),
		.hIconSm = nullptr
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

Window::Window(int width, int height, const char* name) noexcept
{
	static std::vector<Window*> windows;
	this->width = width;
	this->height = height;
	this->name = name;
	exitCode = 0;
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	hWnd = CreateWindow(WindowTemplate::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowTemplate::GetInstance(), this);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	Window::windowCount++;
	index = Window::windows.size();
	Window::windows.push_back(this);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

bool Window::AreActiveWindows()
{
	return windowCount > 0;
}

int Window::DeconstructWindow(int index)
{
	int ret = Window::windows[index]->exitCode;
	Window::windows[index]->~Window();
	Window::windowCount--;
	return ret;
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  noexcept
{
	return reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	std::ostringstream oss;
	switch (msg)
	{
	case WM_CLOSE:
		oss << "closed " << name << '\n';
		OutputDebugString(oss.str().c_str());
		exitCode = 0;
		PostQuitMessage(index);
		return 0;
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
	return DefWindowProc(hWnd, msg, wParam, lParam);
}