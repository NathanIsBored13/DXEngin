#pragma once

#include "Includes.h"
#include "WinMain.h"

#include <sstream>

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	const char *pClassName = "MainWindow";

	WNDCLASSEX wnd =
	{
		.cbSize = sizeof(wnd),
		.style = CS_OWNDC,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = nullptr,
		.hCursor = nullptr,
		.hbrBackground = nullptr,
		.lpszMenuName = nullptr,
		.lpszClassName = pClassName,
		.hIconSm = nullptr
	};
	RegisterClassEx(&wnd);
	HWND hWnd = CreateWindowEx(0, pClassName, "Test", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 0, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hWnd, SW_SHOW);
	MSG msg;
	BOOL gResult;
	while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return gResult == -1 ? -1 : msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wPram, LPARAM lPram)
{
	std::ostringstream oss;
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:

		break;
	case WM_CHAR:
		oss << wPram << '\n' << static_cast<char>(wPram) << '\n';
		OutputDebugString(oss.str().c_str());
		break;
	case WM_LBUTTONDOWN:
		POINTS pt = MAKEPOINTS(lPram);
		oss << '(' << pt.x << ", " << pt.y << ")\n";
		OutputDebugString(oss.str().c_str());
		break;
	}
	return DefWindowProc(hwnd, msg, wPram, lPram);
}