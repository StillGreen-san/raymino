#pragma once

#include <functional>
#include <vector>

namespace raymino
{
struct Timer
{
	bool tick(float delta)
	{
		elapsed += delta;
		if(elapsed >= delay)
		{
			elapsed -= delay;
			return true;
		}
		return false;
	}
	float delay;
	float elapsed = 0;
};
} // namespace raymino
