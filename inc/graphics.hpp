#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <raylib.h>

#include <vector>

namespace raymino
{
/**
 * @brief a fixed mapping between Color<->Grid::Cell
 */
class ColorMap
{
public:
	ColorMap() = delete;
	explicit ColorMap(std::vector<::Color> colors) noexcept;
	::Color operator[](Grid::Cell idx) const noexcept;
	Grid::Cell operator[](::Color color) const noexcept;

private:
	std::vector<::Color> colors;
};

/**
 * @param grid to draw (non empty cells of)
 * @param at position
 * @param cellSize
 * @param borderSize
 * @param minoColors
 * @param alpha
 * @remarks total size = (grid.size * cellSize) + ((grid.size - 1) * borderSize)
 */
void drawCells(
    const Grid& grid, XY at, int cellSize, int borderSize, const ColorMap& minoColors, uint8_t alpha = 255) noexcept;

/**
 * @param grid to draw background of
 * @param at position
 * @param cellSize
 * @param borderSize
 * @param fill color
 * @param lines color
 * @remarks total size = (grid.size * cellSize) + ((grid.size - 1) * borderSize)
 */
void drawBackground(const Grid& grid, XY at, int cellSize, int borderSize, ::Color fill, ::Color lines) noexcept;
} // namespace raymino
