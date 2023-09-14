#include "input.hpp"

#include <raylib-cpp.hpp>

namespace raymino
{
KeyAction::Return KeyAction::tick(float delta) noexcept
{
	if(const int val = (::IsKeyPressed(lkey) ? -1 : 0) + (::IsKeyPressed(rkey) ? 1 : 0); val != 0)
	{
		delayTimer.elapsed = -repeatDelay;
		return {State::Pressed, static_cast<int8_t>(val)};
	}

	if(const int val = (::IsKeyDown(lkey) ? -1 : 0) + (::IsKeyDown(rkey) ? 1 : 0); val != 0 && delayTimer.step(delta))
	{
		return {State::Repeated, static_cast<int8_t>(val)};
	}

	if(const int val = (::IsKeyReleased(lkey) ? -1 : 0) + (::IsKeyReleased(rkey) ? 1 : 0); val != 0)
	{
		return {State::Released, static_cast<int8_t>(val)};
	}

	return {State::None, 0};
}
} // namespace raymino
