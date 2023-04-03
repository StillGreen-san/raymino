#include "input.hpp"

namespace raymino
{
KeyAction::Return KeyAction::tick(float delta)
{
	if(const int val = (::IsKeyPressed(lkey) ? -1 : 0) + (::IsKeyPressed(rkey) ? 1 : 0); val != 0)
	{
		holdDelay.elapsed = 0;
		return {State::Pressed, static_cast<int8_t>(val)};
	}

	if(const int val = (::IsKeyDown(lkey) ? -1 : 0) + (::IsKeyDown(rkey) ? 1 : 0); val != 0 && holdDelay.tick(delta))
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