#pragma once

#include "Includes.h"

#include <queue>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			MPress,
			MRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event() noexcept;
		Event(Type type, const Mouse& parent) noexcept;
		bool IsValid() const noexcept;
		Type GetType() const noexcept;
		std::pair<int, int> GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool LeftIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;
	};
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}
	void Flush() noexcept;
private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnMiddlePressed(int x, int y) noexcept;
	void OnMiddleReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;
	static constexpr unsigned int bufferSize = 16u;
	int x = 0;
	int y = 0;
	bool leftIsPressed = false;
	bool middleIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};

