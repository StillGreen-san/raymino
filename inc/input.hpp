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
	KeyAction(float repeatDelay, float repeatRate, ::KeyboardKey rkey, ::KeyboardKey lkey = KEY_NULL) :
	    repeatDelay{repeatDelay}, delayTimer{repeatRate}, rkey{rkey}, lkey{lkey}
	{
	}
	Return tick(float delta);
	float repeatDelay;
	Timer delayTimer;
	::KeyboardKey rkey;
	::KeyboardKey lkey;
};
} // namespace raymino
