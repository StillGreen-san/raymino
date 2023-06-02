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
constexpr int SIDEBAR_WIDTH = 150;
constexpr int PREVIEW_ELEMENT_HEIGHT = 100;
constexpr int PREVIEW_CELL_SIZE = 30;
constexpr int FIELD_BORDER_WIDTH = 2;
constexpr XY OFFSCREEN_POSITION{-666, -666};

const ColorMap minoColors{
    {LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE,
        DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN, WHITE, BLACK, BLANK, MAGENTA, RAYWHITE}};

void prepareTetromino(Tetromino& tetromino, Grid::Cell color, int fieldWidth)
{
	tetromino.collision.transformCells(
	    [color](Grid::Cell current)
	    {
		    return static_cast<Grid::Cell>(current * color);
	    });
	tetromino.position = spawnPosition(tetromino, HIDDEN_HEIGHT, fieldWidth);
}

std::vector<Tetromino> prepareTetrominos(std::vector<Tetromino> tetrominos, int fieldWidth)
{
	std::sort(tetrominos.begin(), tetrominos.end(),
	    [](const Tetromino& lhs, const Tetromino& rhs)
	    {
		    return lhs.type < rhs.type;
	    });
	prepareTetromino(tetrominos[static_cast<size_t>(TetrominoType::I)], minoColors[SKYBLUE], fieldWidth);
	prepareTetromino(tetrominos[static_cast<size_t>(TetrominoType::L)], minoColors[ORANGE], fieldWidth);
	prepareTetromino(tetrominos[static_cast<size_t>(TetrominoType::J)], minoColors[BLUE], fieldWidth);
	prepareTetromino(tetrominos[static_cast<size_t>(TetrominoType::O)], minoColors[YELLOW], fieldWidth);
	prepareTetromino(tetrominos[static_cast<size_t>(TetrominoType::Z)], minoColors[RED], fieldWidth);
	prepareTetromino(tetrominos[static_cast<size_t>(TetrominoType::T)], minoColors[PINK], fieldWidth);
	prepareTetromino(tetrominos[static_cast<size_t>(TetrominoType::S)], minoColors[GREEN], fieldWidth);
	return tetrominos;
}

Rect calculatePlayfieldBounds(Size fieldSize)
{
	const int availableWidth = ((App::Settings::SCREEN_WIDTH - (SIDEBAR_WIDTH * 2)) - (FIELD_BORDER_WIDTH * 2));
	const int availableHeight = (App::Settings::SCREEN_HEIGHT - (FIELD_BORDER_WIDTH * 2));
	const int preferredFromWidth = availableWidth / fieldSize.width;
	const int preferredFromHeight = availableHeight / fieldSize.height;
	const int cellSize = std::min(preferredFromWidth, preferredFromHeight);
	const int actualWidth = cellSize * fieldSize.width;
	const int actualHeight = cellSize * fieldSize.height;
	const int xOffset = (SIDEBAR_WIDTH + FIELD_BORDER_WIDTH) + ((availableWidth - actualWidth) / 2);
	const int yOffset = FIELD_BORDER_WIDTH + ((availableHeight - actualHeight) / 2);

	return {xOffset, yOffset, actualWidth, actualHeight};
}

XY calcCenterOffset(const Grid& grid, Size available, int cellSize)
{
	const Rect actual = findTrueSize(grid) * cellSize;
	const int xOffset = ((available.width - actual.width) / 2) - actual.x;
	const int yOffset = ((available.height - actual.height) / 2) - actual.y;
	return {xOffset, yOffset};
}

void Game::update(App& app)
{
	if(IsKeyPressed(KEY_END))
	{
		app.QueueSceneSwitch(MakeScene<Scene::Menu>(app));
	}
	if(app.settings.holdPiece && IsKeyPressed(KEY_H))
	{
		holdPieceIdx = (holdPieceIdx + 1) % baseTetrominos.size();
		holdPiecePosition = calcCenterOffset(
		    baseTetrominos[holdPieceIdx].collision, {SIDEBAR_WIDTH, PREVIEW_ELEMENT_HEIGHT}, PREVIEW_CELL_SIZE);
	}
}

void Game::draw()
{
	::BeginDrawing();

	::ClearBackground(LIGHTGRAY);

	const raylib::Rectangle playfieldBorderBounds(static_cast<float>(playfieldBounds.x - FIELD_BORDER_WIDTH),
	    static_cast<float>(playfieldBounds.y - FIELD_BORDER_WIDTH),
	    static_cast<float>(playfieldBounds.width + (FIELD_BORDER_WIDTH * 2) - 1),
	    static_cast<float>(playfieldBounds.height + (FIELD_BORDER_WIDTH * 2) - 1));
	::DrawRectangleLinesEx(playfieldBorderBounds, FIELD_BORDER_WIDTH, DARKGRAY);
	const int cellSize = (playfieldBounds.width / playfield.getSize().width) - 1;
	drawBackground(playfield, playfieldBounds, cellSize, 1, LIGHTGRAY, DARKGRAY);
	drawCells(playfield, playfieldBounds, cellSize, 1, minoColors);

	drawCells(baseTetrominos[holdPieceIdx].collision, holdPiecePosition, PREVIEW_CELL_SIZE, 1, minoColors);

	::EndDrawing();
}

void Game::UpdateDraw(App& app)
{
	update(app);
	draw();
}

Game::Game(App& app) :
    playfield{{app.settings.fieldWidth, app.settings.fieldHeight}, 0},
    playfieldBounds{calculatePlayfieldBounds(playfield.getSize())},
    baseTetrominos{prepareTetrominos(makeBaseMinos(app.settings.rotationSystem)(), playfield.getSize().width)},
    holdPieceIdx{0}, holdPiecePosition{OFFSCREEN_POSITION}
{
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>([[maybe_unused]] App& app)
{
	return std::make_unique<Game>(app);
}
} // namespace raymino
