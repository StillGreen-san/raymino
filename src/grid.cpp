#include "grid.hpp"

#include "types.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace raymino
{
size_t index1D(unsigned x, unsigned y, unsigned width)
{
	return (y * width) + x;
}

Grid::Grid(Size size, uint8_t fill) : cells(size.area(), fill), size{size}
{
}

Grid::Grid(Size size, const std::vector<uint8_t>& grid) : size{size}
{
	if(grid.size() != size.area())
	{
		throw std::logic_error("size mismatch");
	}
	cells = grid;
}

Grid::Grid(const Grid& other, std::function<TTransformFunc> func) : size{other.size}
{
	cells.reserve(other.cells.size());
	std::transform(other.cells.begin(), other.cells.end(), std::back_insert_iterator(cells), std::move(func));
}

bool Grid::overlapAt(XY topLeft, const Grid& other) const
{
	for(int y = 0; y < other.size.height; ++y)
	{
		for(int x = 0; x < other.size.width; ++x)
		{
			const unsigned check = other.cells[index1D(x, y, other.size.width)];
			if(getAt({x + topLeft.x, y + topLeft.y}) + check > check)
			{
				return true;
			}
		}
	}
	return false;
}

Size Grid::getSize() const
{
	return size;
}

uint8_t Grid::getAt(XY topLeft) const
{
	if(topLeft.x < 0 || topLeft.x >= size.width || topLeft.y >= size.height)
	{
		return 0xFF;
	}
	return cells[index1D(topLeft.x, topLeft.y, size.width)];
}

void Grid::setAt(XY topLeft, const Grid& other)
{
	for(int y = 0; y < other.size.height; ++y)
	{
		for(int x = 0; x < other.size.width; ++x)
		{
			if(topLeft.x + x >= size.width)
			{
				break;
			}
			if(topLeft.x + x > 0 && topLeft.y + y < size.height)
			{
				cells[index1D(x + topLeft.x, y + topLeft.y, other.size.width)] =
				    other.cells[index1D(x, y, other.size.width)];
			}
		}
	}
}

void Grid::transformCells(std::function<TTransformFunc> func)
{
	std::transform(cells.begin(), cells.end(), cells.begin(), std::move(func));
}
} // namespace raymino
