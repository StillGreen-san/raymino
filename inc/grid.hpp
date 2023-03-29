#pragma once

#include "types.hpp"

#include <functional>
#include <vector>

namespace raymino
{
class Grid
{
public:
	using TOverlapFunc = bool(uint8_t lhs, uint8_t rhs);
	using TTransformFunc = uint8_t(uint8_t);
	Grid() = delete;
	Grid(Size size, uint8_t fill);
	/**
	 * @throws std::logic_error on size mismatch
	 */
	Grid(Size size, const std::vector<uint8_t>& grid);
	Grid(const Grid& other, std::function<TTransformFunc> func);
	Grid subGrid(Rect rect) const;
	bool overlapAt(XY topLeft, const Grid& other) const;
	bool isSqaure() const;
	Size getSize() const;
	uint8_t getAt(XY topLeft) const;
	void transformCells(std::function<TTransformFunc> func);
	void rotate(int steps);
	void transpose();
	void reverseRows();
	void setAt(XY topLeft, const Grid& other);
	[[nodiscard]] auto begin()
	{
		return cells.begin();
	}
	[[nodiscard]] auto end()
	{
		return cells.end();
	}
	[[nodiscard]] auto begin() const
	{
		return cells.begin();
	}
	[[nodiscard]] auto end() const
	{
		return cells.end();
	}
	[[nodiscard]] auto rbegin() const
	{
		return cells.rbegin();
	}
	[[nodiscard]] auto rend() const
	{
		return cells.rend();
	}
	[[nodiscard]] auto rbegin()
	{
		return cells.rbegin();
	}
	[[nodiscard]] auto rend()
	{
		return cells.rend();
	}

private:
	std::vector<uint8_t> cells;
	Size size;
};
} // namespace raymino
