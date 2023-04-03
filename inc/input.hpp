#pragma once

#include "timer.hpp"

#include <raylib-cpp.hpp>

namespace raymino
{
struct KeyAction
{
	enum class State : uint8_t
	{
		None,
		Pressed,
		Repeated,
		Released,
	};
	struct Return
	{
		State state;
		int8_t value;
	};
	KeyAction() = delete;
	KeyAction(float holdDelay, KeyboardKey rkey, KeyboardKey lkey = KEY_NULL) :
	    holdDelay{holdDelay}, rkey{rkey}, lkey{lkey}
	{
	}
	Return tick(float delta);
	Timer holdDelay;
	::KeyboardKey rkey;
	::KeyboardKey lkey;
};
} // namespace raymino
