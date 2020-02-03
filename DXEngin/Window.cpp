#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
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

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* name) noexcept
{
	this->width = width;
	this->height = height;
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	hWnd = CreateWindow(WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
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
	Window* const pWind = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWind->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	std::ostringstream oss;
	char buffer[256];
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		GetWindowText(hWnd, buffer, sizeof(buffer));
		oss << "closed " << buffer << '\n';
		OutputDebugString(oss.str().c_str());
		this->~Window();
		return 0;
		break;
	case WM_KEYDOWN:

		break;
	case WM_CHAR:
		GetWindowText(hWnd, buffer, sizeof(buffer));
		oss << buffer << ": " << wParam << '\n' << static_cast<char>(wParam) << '\n';
		OutputDebugString(oss.str().c_str());
		break;
	case WM_LBUTTONDOWN:
		POINTS pt = MAKEPOINTS(lParam);
		GetWindowText(hWnd, buffer, sizeof(buffer));
		oss << buffer << ": (" << pt.x << ", " << pt.y << ") [" << width << ", " << height << "]\n";
		OutputDebugString(oss.str().c_str());
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}