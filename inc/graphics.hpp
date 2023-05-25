#pragma once

#include "grid.hpp"
#include "types.hpp"

#include <raylib-cpp.hpp>

#include <vector>

namespace raymino
{
/**
 * @param grid to draw (non empty cells of)
 * @param at position
 * @param cellSize
 * @param borderSize
 * @param minoColor
 * @remarks total size = (grid.size * cellSize) + ((grid.size - 1) * borderSize)
 */
void drawCells(const Grid& grid, XY at, int cellSize, int borderSize, Color minoColor);

/**
 * @param grid to draw background of
 * @param at position
 * @param cellSize
 * @param borderSize
 * @param fill color
 * @param lines color
 * @remarks total size = (grid.size * cellSize) + ((grid.size - 1) * borderSize)
 */
void drawBackground(const Grid& grid, XY at, int cellSize, int borderSize, Color fill, Color lines);
} // namespace raymino
