#pragma once

#include "types.hpp"

#include <functional>
#include <vector>

namespace raymino
{
class Grid
{
public:
	using Cell = uint8_t;
	using TTransformFunc = Cell(Cell);
	static constexpr Cell oobVal = 0xFF;
	Grid() = delete;
	Grid(Size size, Cell fill);
	/**
	 * @throws std::logic_error on size mismatch
	 */
	Grid(Size size, const std::vector<Cell>& grid);
	Grid(const Grid& other, std::function<TTransformFunc> func);

	/**
	 * @param topLeft XY offset for other inside this
	 * @param other Grid to test against
	 * @return size_t 1 based index into other where overlap occurred
	 */
	[[nodiscard]] size_t overlapAt(XY topLeft, const Grid& other) const noexcept;

	[[nodiscard]] bool isSquare() const noexcept
	{
		return size.width == size.height;
	}

	[[nodiscard]] Size getSize() const noexcept
	{
		return size;
	}

	/**
	 * @param topLeft offset
	 * @param oobValue = Grid::oobVal
	 * @return Grid::Cell cell value
	 */
	[[nodiscard]] Cell getAt(XY topLeft, Cell oobValue = oobVal) const noexcept;

	void transformCells(std::function<TTransformFunc> func) noexcept;
	void rotate(int steps) noexcept;
	void transpose() noexcept;
	void reverseRows() noexcept;
	void setAt(XY topLeft, const Grid& other) noexcept;
	[[nodiscard]] auto begin() noexcept
	{
		return cells.begin();
	}
	[[nodiscard]] auto end() noexcept
	{
		return cells.end();
	}
	[[nodiscard]] auto begin() const noexcept
	{
		return cells.begin();
	}
	[[nodiscard]] auto end() const noexcept
	{
		return cells.end();
	}
	[[nodiscard]] auto rbegin() const noexcept
	{
		return cells.rbegin();
	}
	[[nodiscard]] auto rend() const noexcept
	{
		return cells.rend();
	}
	[[nodiscard]] auto rbegin() noexcept
	{
		return cells.rbegin();
	}
	[[nodiscard]] auto rend() noexcept
	{
		return cells.rend();
	}
	[[nodiscard]] bool operator==(const Grid& other) noexcept
	{
		return size == other.size && cells == other.cells;
	}

private:
	std::vector<Cell> cells;
	Size size;
};
} // namespace raymino
