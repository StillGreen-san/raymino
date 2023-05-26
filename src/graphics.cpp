#include "graphics.hpp"

#include <raylib.h>

bool constexpr operator<(Color lhs, Color rhs) noexcept
{
	return std::tie(lhs.r, lhs.g, lhs.b, lhs.a) < std::tie(rhs.r, rhs.g, rhs.b, rhs.a);
}

#include <algorithm>

namespace raymino
{
ColorMap::ColorMap(std::vector<Color> colors) : colors{std::move(colors)}
{
	std::sort(this->colors.begin(), this->colors.end());
}
Color ColorMap::operator[](Grid::Cell idx) const
{
	return colors[idx];
}
Grid::Cell ColorMap::operator[](Color color) const
{
	auto idxIt = std::lower_bound(colors.begin(), colors.end(), color);
	return static_cast<Grid::Cell>(std::distance(begin(colors), idxIt));
}

void drawCells(const Grid& grid, XY at, int cellSize, int borderSize, const ColorMap& minoColors)
{
	const Size gridSize = grid.getSize();

	if(gridSize.area() == 0)
	{
		return;
	}

	const int totalCellSize = cellSize + borderSize;
	const int totalGridWidth = totalCellSize * gridSize.width;
	const int maxGridOffset = at.x + totalGridWidth;
	for(const Grid::Cell cell : grid)
	{
		if(cell != 0)
		{
			::DrawRectangle(at.x, at.y, cellSize, cellSize, minoColors[cell]);
		}
		at.x += totalCellSize;
		if(at.x == maxGridOffset)
		{
			at.x -= totalGridWidth;
			at.y += totalCellSize;
		}
	}
}

void drawBackground(const Grid& grid, XY at, int cellSize, int borderSize, Color fill, Color lines)
{
	const Size gridSize = grid.getSize();

	if(gridSize.area() == 0)
	{
		return;
	}

	const int totalWidth = (gridSize.width * cellSize) + ((gridSize.width - 1) * borderSize);
	const int totalHeight = (gridSize.height * cellSize) + ((gridSize.height - 1) * borderSize);

	::DrawRectangle(at.x, at.y, totalWidth, totalHeight, fill);

	for(int vLine = 1; vLine < gridSize.width; ++vLine)
	{
		const int xOffset = (vLine * cellSize) + ((vLine - 0) * borderSize);
		::DrawLine(at.x + xOffset, at.y, at.x + xOffset, at.y + totalHeight, lines);
	}
	for(int hLine = 1; hLine < gridSize.height; ++hLine)
	{
		const int yOffset = (hLine * cellSize) + ((hLine - 1) * borderSize);
		::DrawLine(at.x, at.y + yOffset, at.x + totalWidth, at.y + yOffset, lines);
	}
}
} // namespace raymino
