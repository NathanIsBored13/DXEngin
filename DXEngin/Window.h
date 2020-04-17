#pragma once

#include "Includes.h"
#include "ExtendedException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "resource.h"

#include <memory>
#include <optional>

class Window
{
public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Graphics& GetGFX();
	void SetWindowTitle(const char* title) noexcept;
	const char* GetWindowTitle() noexcept;
	static bool IsActiveWindow() noexcept;
	static std::optional<int> ProcessMessages() noexcept;
	Keyboard kbd;
	Mouse mouse;
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
	LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;
	static LRESULT CALLBACK HandleMsgSetup(HWND, UINT, WPARAM, LPARAM) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND, UINT, WPARAM, LPARAM) noexcept;
	HWND hWnd;
	std::unique_ptr<Graphics> gfx;
	static int wndCount;
	const char* name;
	int width, height;
};