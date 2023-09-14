#pragma once

#include <array>
#include <cstddef>

namespace raymino
{
struct Timer
{
	/**
	 * @brief accumulate delta, will decrease elapsed until elapsed < delay
	 * @param delta seconds
	 * @return true if elapsed became >= delay
	 */
	bool step(float delta) noexcept
	{
		elapsed += delta;
		if(elapsed >= delay)
		{
			while(elapsed >= delay)
			{
				elapsed -= delay;
			}
			return true;
		}
		return false;
	}
	/**
	 * @brief accumulate delta, needs manual reset
	 * @param delta seconds
	 * @return true if elapsed is >= delay
	 */
	bool tick(float delta) noexcept
	{
		elapsed += delta;
		return elapsed >= delay;
	}
	/**
	 * @brief reset accumulated time to val
	 */
	void reset(float val) noexcept
	{
		elapsed = val;
	}
	float delay;
	float elapsed = 0;
};

constexpr std::array delays{
    1.f / 1,
    1.f / 2,
    1.f / 3,
    1.f / 4,
    1.f / 5,
    1.f / 6,
    1.f / 10,
    1.f / 12,
    1.f / 15,
    1.f / 20,
    1.f / 30,
    1.f / 60,
    1.f / 120,
    1.f / 180,
    1.f / 240,
    1.f / 300,
    1.f / 360,
    1.f / 420,
    1.f / 480,
    1.f / 540,
    1.f / 600,
    1.f / 660,
    1.f / 720,
    1.f / 780,
    1.f / 840,
    1.f / 900,
    1.f / 960,
    1.f / 1020,
    1.f / 1080,
    1.f / 1140,
    1.f / 1200,
};
constexpr size_t maxSpeedLevel = delays.size() - 1;
} // namespace raymino
