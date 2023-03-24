#include "grid.hpp"

#include "types.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace raymino
{
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

void Grid::transformCells(std::function<TTransformFunc> func)
{
	std::transform(cells.begin(), cells.end(), cells.begin(), std::move(func));
}
} // namespace raymino
