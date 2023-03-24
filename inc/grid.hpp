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
	bool overlapAt(XY topLeft, const Grid& other, std::function<TOverlapFunc> check) const;
	bool isSqaure() const;
	Size getSize() const;
	void transformCells(std::function<TTransformFunc> func);
	void rotate(int steps);
	void setAt(XY topLeft, const Grid& other);
	std::vector<uint8_t>::iterator begin();
	std::vector<uint8_t>::iterator end();

private:
	std::vector<uint8_t> cells;
	Size size;
};
} // namespace raymino
