#include "game.hpp"

#include "app.hpp"
#include "gameplay.hpp"
#include "graphics.hpp"
#include "grid.hpp"
#include "scenes.hpp"
#include "timer.hpp"
#include "types.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

bool constexpr operator<(Color lhs, Color rhs) noexcept
{
	return std::tie(lhs.r, lhs.g, lhs.b, lhs.a) < std::tie(rhs.r, rhs.g, rhs.b, rhs.a);
}

#include <algorithm>
#include <charconv>
#include <random>
#include <vector>

namespace raymino
{
constexpr int HIDDEN_HEIGHT = 4;

const ColorMap minoColors{
    {LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE,
        DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN, WHITE, BLACK, BLANK, MAGENTA, RAYWHITE}};

void drawPreview(Range<Playfield::MinoConstIterator> range, XY at, int firstCellSize, int restCellSize)
{
	drawCells(*range.first, at, firstCellSize, 1, minoColors);
	at.y += (range.first->getSize().height * firstCellSize) + restCellSize;
	++range.first;
	for(const Grid& mino : range)
	{
		drawCells(mino, at, restCellSize, 1, minoColors);
		at.y += (mino.getSize().height * restCellSize) + restCellSize;
	}
}

void prepareTetromino(Tetromino& tetromino, Grid::Cell color, int fieldWidth)
{
	tetromino.collision.transformCells(
	    [color](Grid::Cell current)
	    {
		    return current * color;
	    });
	tetromino.position = spawnPosition(tetromino, HIDDEN_HEIGHT, fieldWidth);
}

std::vector<Tetromino> prepareTetrominos(std::vector<Tetromino>& tetrominos, int fieldWidth)
{
	prepareTetromino(*find(tetrominos, TetrominoType::I), minoColors[SKYBLUE], fieldWidth);
	prepareTetromino(*find(tetrominos, TetrominoType::J), minoColors[BLUE], fieldWidth);
	prepareTetromino(*find(tetrominos, TetrominoType::L), minoColors[ORANGE], fieldWidth);
	prepareTetromino(*find(tetrominos, TetrominoType::O), minoColors[YELLOW], fieldWidth);
	prepareTetromino(*find(tetrominos, TetrominoType::S), minoColors[GREEN], fieldWidth);
	prepareTetromino(*find(tetrominos, TetrominoType::T), minoColors[PINK], fieldWidth);
	prepareTetromino(*find(tetrominos, TetrominoType::Z), minoColors[RED], fieldWidth);
	return tetrominos;
}

void Game::update([[maybe_unused]] App& app)
{
}

void Game::draw()
{
}

void Game::UpdateDraw(App& app)
{
	update(app);
	draw();
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>([[maybe_unused]] App& app)
{
	return std::make_unique<Game>();
}
} // namespace raymino
