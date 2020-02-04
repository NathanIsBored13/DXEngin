#pragma once

#include "Includes.h"

#include <sstream>

class Window
{
public:
	Window(int width, int height, const char* name) noexcept;
	~Window();
	static int GetWindowCount();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static int windowCount;
	int width;
	int height;
	const char* name;
	HWND hWnd;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND, UINT, WPARAM, LPARAM) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND, UINT, WPARAM, LPARAM) noexcept;
	LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "DX3DEngin";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
};