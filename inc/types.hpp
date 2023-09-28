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
	Size operator*(int val) const noexcept
	{
		return {width * val, height * val};
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
		return {XY::operator*(val), Size::operator*(val)};
	}
};
template<typename TIterator>
struct Range
{
	TIterator first;
	TIterator last;
	template<typename TContainer>
	explicit Range(TContainer& container) noexcept : first{std::begin(container)}, last{std::end(container)}
	{
	}
	[[nodiscard]] auto begin() noexcept
	{
		return first;
	}
	[[nodiscard]] auto end() noexcept
	{
		return last;
	}
	[[nodiscard]] auto begin() const noexcept
	{
		return first;
	}
	[[nodiscard]] auto end() const noexcept
	{
		return last;
	}
};
template<typename TContainer>
Range(TContainer& container) -> Range<decltype(std::begin(container))>;

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
	Original = 0,
	Super = 1,
	Arika = 2,
	Sega = 3,
	NintendoLeft = 4,
	NintendoRight = 5,
};
enum class WallKicks : uint8_t
{
	None = 0,
	Arika = 1,
	Super = 2,
};
enum class LockDown : uint8_t
{
	Infinit = 0,
	Extended = 1,
	Classic = 2,
	Entry = 3,
	Instant = 4,
};
enum class SoftDrop : uint8_t
{
	Locking = 0,
	NonLocking = 1,
};
enum class InstantDrop : uint8_t
{
	Hard = 0,
	Sonic = 1,
	None = 2,
};
enum class TSpin : uint8_t
{
	Immobile = 0,
	ThreeCorner = 1,
	Lenient = 2,
	None = 3,
};
enum class ShuffleType : uint8_t
{
	Random = 0,
	SingleBag = 1,
	DoubleBag = 2,
	TripleBag = 3,
	TGMH4 = 4,
	TGM35 = 5,
	NES = 6,
};
enum class ScoringSystem : uint8_t
{
	Guideline = 0,
	BPS = 1,
	Sega = 2,
	Nintendo = 3,
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
	Fixed = 0,
	Dynamic = 1,
};
} // namespace raymino