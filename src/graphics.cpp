#include "graphics.hpp"

#include <raylib.h>

bool constexpr operator<(::Color lhs, ::Color rhs) noexcept
{
	return std::tie(lhs.r, lhs.g, lhs.b, lhs.a) < std::tie(rhs.r, rhs.g, rhs.b, rhs.a);
}

#include <algorithm>

namespace raymino
{
ColorMap::ColorMap(std::vector<::Color> colors) noexcept : colors{std::move(colors)}
{
	std::sort(this->colors.begin(), this->colors.end());
}
::Color ColorMap::operator[](Grid::Cell idx) const noexcept
{
	return colors[idx];
}
Grid::Cell ColorMap::operator[](::Color color) const noexcept
{
	auto idxIt = std::lower_bound(colors.begin(), colors.end(), color);
	return static_cast<Grid::Cell>(std::distance(begin(colors), idxIt));
}

void drawCells(
    const Grid& grid, XY at, int cellSize, int borderSize, const ColorMap& minoColors, uint8_t alpha) noexcept
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
			::Color color = minoColors[cell];
			color.a = alpha;
			::DrawRectangle(at.x, at.y, cellSize, cellSize, color);
		}
		at.x += totalCellSize;
		if(at.x == maxGridOffset)
		{
			at.x -= totalGridWidth;
			at.y += totalCellSize;
		}
	}
}

void drawBackground(const Grid& grid, XY at, int cellSize, int borderSize, ::Color fill, ::Color lines) noexcept
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
		const float magicYOffset = 0.5f; // + 0.5f offset needed on some platforms for some reason
		::DrawLineEx({static_cast<float>(at.x), static_cast<float>(at.y + yOffset) + magicYOffset},
		    {static_cast<float>(at.x + totalWidth), static_cast<float>(at.y + yOffset) + magicYOffset}, 1.f, lines);
	}
}
} // namespace raymino
