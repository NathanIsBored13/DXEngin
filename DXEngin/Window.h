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
	struct WindowStatus
	{
		bool active;
		int exitCode;
	};
	Window(int, int, const char*);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	static bool IsActiveWindow() noexcept;
	static std::optional<int> ProcessMessages() noexcept;
	static int OnWindowQuit(int) noexcept;
	virtual void DoFrame(float) = 0;
	bool IsWindowActive() noexcept;
	void SetWindowTitle(const char*) noexcept;
	const char* GetWindowTitle() noexcept;
	int GetExitCode() noexcept;
	Graphics& GetGFX();
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
	static LRESULT CALLBACK HandleMsgSetup(HWND, UINT, WPARAM, LPARAM) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND, UINT, WPARAM, LPARAM) noexcept;
	LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;
	void PostQuit(int) noexcept;
	static std::vector<WindowStatus> statuses;
	static int wndCount;
	HWND hWnd;
	std::unique_ptr<Graphics> gfx;
	const char* name;
	int width, height, index;
};