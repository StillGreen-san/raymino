#include "grid.hpp"

#include "types.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace raymino
{
size_t index1D(int xPos, int yPos, int width) noexcept
{
	return (static_cast<size_t>(yPos) * static_cast<size_t>(width)) + static_cast<size_t>(xPos);
}

Grid::Grid(Size size, Grid::Cell fill) : cells(static_cast<size_t>(size.area()), fill), size{size}
{
}

Grid::Grid(Size size, const std::vector<Grid::Cell>& grid) : size{size}
{
	if(grid.size() != static_cast<size_t>(size.area()))
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

size_t Grid::overlapAt(XY topLeft, const Grid& other) const noexcept
{
	for(int yPos = 0; yPos < other.size.height; ++yPos)
	{
		for(int xPos = 0; xPos < other.size.width; ++xPos)
		{
			const auto otherCell = other.getAt({xPos, yPos});
			const auto thisCell = getAt({xPos + topLeft.x, yPos + topLeft.y});
			if(thisCell != 0 && otherCell != 0)
			{
				return index1D(xPos, yPos, other.size.width) + 1;
			}
		}
	}
	return 0;
}

Grid::Cell Grid::getAt(XY topLeft, Grid::Cell oobValue) const noexcept
{
	if(topLeft.x < 0 || topLeft.x >= size.width || topLeft.y >= size.height || topLeft.y < 0)
	{
		return oobValue;
	}
	return cells[index1D(topLeft.x, topLeft.y, size.width)];
}

void Grid::setAt(XY topLeft, const Grid& other) noexcept
{
	for(int yPos = 0; yPos < other.size.height; ++yPos)
	{
		if(topLeft.y + yPos < 0)
		{
			continue;
		}
		for(int xPos = 0; xPos < other.size.width; ++xPos)
		{
			if(topLeft.x + xPos >= size.width)
			{
				continue;
			}
			if(topLeft.x + xPos >= 0 && topLeft.y + yPos < size.height)
			{
				const auto thisIndex = index1D(xPos + topLeft.x, yPos + topLeft.y, size.width);
				const auto otherIndex = index1D(xPos, yPos, other.size.width);
				cells[thisIndex] |= other.cells[otherIndex];
			}
		}
	}
}

void Grid::rotate(int steps) noexcept
{
	steps %= 4;
	while(steps != 0)
	{
		if(steps < 0)
		{
			reverseRows();
			transpose();
			++steps;
		}
		else
		{
			transpose();
			reverseRows();
			--steps;
		}
	}
}

void Grid::transpose() noexcept
{
	if(!isSquare())
	{
		std::swap(size.width, size.height);
	}
	for(int i = 0; i < size.area(); ++i)
	{
		auto dest = (size.height * (i % size.width)) + (i / size.width);
		while(dest < i)
		{
			dest = (size.height * (dest % size.width)) + (dest / size.width);
		}
		std::swap(cells[i], cells[dest]);
	}
}

void Grid::reverseRows() noexcept
{
	for(ptrdiff_t row = 0; row < size.height; ++row)
	{
		std::reverse(next(cells.begin(), row * size.width), next(cells.begin(), (row + 1) * size.width));
	}
}

void Grid::transformCells(std::function<TTransformFunc> func) noexcept
{
	std::transform(cells.begin(), cells.end(), cells.begin(), std::move(func));
}
} // namespace raymino
