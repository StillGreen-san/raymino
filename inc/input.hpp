#pragma once

#include "timer.hpp"

#include <cstdint>

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
	KeyAction(float repeatDelay, float repeatRate, int16_t rkey, int16_t lkey = 0) :
	    repeatDelay{repeatDelay}, delayTimer{repeatRate}, rkey{rkey}, lkey{lkey}
	{
	}

	/**
	 * @brief check key state
	 * @param delta seconds
	 * @return key state & value
	 */
	Return tick(float delta);

	float repeatDelay;
	Timer delayTimer;
	int16_t rkey;
	int16_t lkey;
};
} // namespace raymino
