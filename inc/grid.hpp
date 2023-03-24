#pragma once

#include "types.hpp"

#include <vector>

namespace raymino
{
class Grid
{
public:
	Grid() = delete;
	Grid(Size size, uint8_t fill);
	Grid(Size size, std::vector<uint8_t> grid);
	Grid subGrid(Rect rect) const;
	void rotate(int steps);
	void setAt(XY topLeft, const Grid& other);
	using TOverlapFunc = bool(uint8_t lhs, uint8_t rhs);
	bool overlapAt(XY topLeft, const Grid& other, TOverlapFunc check) const;
	bool isSqaure() const;
	Size getSize() const;
	std::vector<uint8_t>::iterator begin();
	std::vector<uint8_t>::iterator end();
private:
	std::vector<uint8_t> cells;
	Size size;
};
} // namespace raymino
