#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>

namespace raymino
{
struct XY
{
	int x;
	int y;
	XY operator+(XY other) const noexcept
	{
		return {x + other.x, y + other.y};
	}
	XY operator-(XY other) const noexcept
	{
		return {x - other.x, y - other.y};
	}
	XY& operator+=(XY other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	XY& operator-=(XY other) noexcept
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	constexpr bool operator==(XY other) const noexcept
	{
		return std::tie(x, y) == std::tie(other.x, other.y);
	}
	XY operator*(int val) const noexcept
	{
		return {x * val, y * val};
	}
};
struct Size
{
	int width;
	int height;
	[[nodiscard]] ptrdiff_t area() const noexcept
	{
		return static_cast<ptrdiff_t>(width) * height;
	}
	constexpr bool operator==(Size other) const noexcept
	{
		return std::tie(width, height) == std::tie(other.width, other.height);
	}
};
struct Rect : XY, Size
{
	constexpr bool operator==(Rect other) const noexcept
	{
		return std::tie(static_cast<const XY&>(*this), static_cast<const Size&>(*this)) ==
		       std::tie(static_cast<const XY&>(other), static_cast<const Size&>(other));
	}
	Rect operator*(int val) const noexcept
	{
		return {{x * val, y * val}, {width * val, height * val}};
	}
};
template<typename TIterator>
struct Range
{
	TIterator first;
	TIterator last;
	[[nodiscard]] auto begin()
	{
		return first;
	}
	[[nodiscard]] auto end()
	{
		return last;
	}
	[[nodiscard]] auto begin() const
	{
		return first;
	}
	[[nodiscard]] auto end() const
	{
		return last;
	}
};

enum class TetrominoType : uint8_t
{
	I,
	L,
	J,
	O,
	Z,
	T,
	S,
};
enum class RotationSystem : uint8_t
{
	Original,
	Super,
	Arika,
	Sega,
	NintendoLeft,
	NintendoRight,
};
enum class WallKicks : uint8_t
{
	None,
	Arika,
	Super,
};
enum class LockDown : uint8_t
{
	Infinit,
	Extended,
	Classic,
	Entry,
};
enum class SoftDrop : uint8_t
{
	Locking,
	NonLocking,
};
enum class InstantDrop : uint8_t
{
	Hard,
	Sonic,
};
enum class TSpin : uint8_t
{
	Immobile,
	ThreeCorner,
	Lenient,
};
enum class ShuffleType : uint8_t
{
	Random,
	SingleBag,
	DoubleBag,
	TripleBag,
};
enum class ScoringSystem : uint8_t
{
	Guideline,
	BPS,
	Sega,
	Nintendo,
};
enum class ScoreEvent : uint8_t
{
	LineClear,
	PerfectClear, // separat from *Clear & *Spin events
	MiniTSpin,
	TSpin,
	SoftDrop,
	HardDrop,
};
enum class LevelGoal : uint8_t
{
	Fixed,
	Dynamic,
};
} // namespace raymino