#pragma once

#include "Includes.h"
#include "ExtendedException.h"
#include "Keyboard.h"
#include "resource.h"

#include <sstream>
#include<vector>
#include<optional>

class Window
{
public:
	Window(int width, int height, const char* name);
	~Window();
	static bool AreActiveWindows() noexcept;
	static int DeconstructWindow(int index) noexcept;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Keyboard kbd;
private:
	class WindowTemplate
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowTemplate() noexcept;
		~WindowTemplate();
		WindowTemplate(const WindowTemplate&) = delete;
		WindowTemplate& operator=(const WindowTemplate&) = delete;
		static constexpr const char* wndClassName = "DX3DEngin";
		static WindowTemplate wndClass;
		HINSTANCE hInst;
	};
	int width, height, index, exitCode;
	const char* name;
	static int windowCount;
	static std::vector<Window*> windows;
	HWND hWnd;
	static LRESULT CALLBACK HandleMsgSetup(HWND, UINT, WPARAM, LPARAM) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND, UINT, WPARAM, LPARAM) noexcept;
	LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;
};