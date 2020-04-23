#pragma once

#include "Window.h"

#include <vector>
#include <optional>

class WindowManager
{
public:
	static void RegisterWindow(Window*) noexcept;
	static void DoFrames(float) noexcept;
	static bool IsActiveWindow() noexcept;
	static bool IsWindowActive(Window*) noexcept;
	static std::optional<int> ProcessMessages() noexcept;
private:
	static std::vector<Window*> wnds;
};

