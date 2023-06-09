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
#include <string_view>
#include <vector>

namespace raymino
{
constexpr int HIDDEN_HEIGHT = 4;
constexpr int SIDEBAR_WIDTH = 150;
constexpr int PREVIEW_ELEMENT_HEIGHT = 100;
constexpr int PREVIEW_CELL_SIZE = 30;
constexpr int FIELD_BORDER_WIDTH = 2;
constexpr XY OFFSCREEN_POSITION{-1337, -1337};
constexpr int SCORE_FONT_SIZE = 30;
constexpr int STATUS_FONT_SIZE = 50;
constexpr ::Color STATUS_BACKGROUND{77, 77, 77, 222};

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

std::vector<XY> calcCenterOffsets(const std::vector<Tetromino>& tetrominos, Size available, int cellSize)
{
	std::vector<XY> offsets;
	offsets.reserve(tetrominos.size() + 1);
	for(const Tetromino& tetromino : tetrominos)
	{
		offsets.push_back(calcCenterOffset(tetromino.collision, available, cellSize));
	}
	offsets.push_back(OFFSCREEN_POSITION);
	return offsets;
}

std::vector<XY> calcCenterOffsetsExtended(const std::vector<Tetromino>& tetrominos, Size available, int cellSize)
{
	std::vector<XY> offsets = calcCenterOffsets(tetrominos, available, cellSize);
	for(XY& offset : offsets)
	{
		offset.x += App::Settings::SCREEN_WIDTH - SIDEBAR_WIDTH;
	}
	return offsets;
}

bool isKeyPress(KeyAction::Return keyPress)
{
	return keyPress.value != 0 && keyPress.state != KeyAction::State::Released;
}

void Game::update(App& app)
{
	if(::IsKeyPressed(KEY_END))
	{
		app.QueueSceneSwitch(MakeScene<Scene::Menu>(app));
		state = State::GameOver;
	}
	if(::IsKeyPressed(KEY_ESCAPE))
	{
		state = state == State::Paused ? State::Running : State::Paused;
	}

	if(state != State::Running)
	{
		return;
	}

	if(app.settings.holdPiece && ::IsKeyPressed(KEY_C))
	{
		if(holdPieceIdx == -1)
		{
			holdPieceIdx = static_cast<size_t>(currentTetromino.type);
			currentTetromino = getNextTetromino(app.settings.previewCount);
		}
		else
		{
			const auto nextHoldIdx = static_cast<size_t>(currentTetromino.type);
			currentTetromino = baseTetrominos[holdPieceIdx];
			holdPieceIdx = nextHoldIdx;
		}
	}

	Offset prevTetrominoOffset = currentTetromino;

	const size_t delayIdx = std::min<size_t>((::IsKeyDown(KEY_DOWN) ? 2 : 0) + levelState.currentLevel, maxSpeedLevel);
	gravity.delay = delays[delayIdx];

	if(const KeyAction::Return keyPress = moveRight.tick(::GetFrameTime()); isKeyPress(keyPress))
	{
		if(playfield.overlapAt(currentTetromino.position + XY{keyPress.value, 0}, currentTetromino.collision) == 0)
		{
			currentTetromino.position += XY{keyPress.value, 0};
		}
	}
	if(gravity.tick(::GetFrameTime()))
	{
		if(playfield.overlapAt(currentTetromino.position + XY{0, 1}, currentTetromino.collision) == 0)
		{
			currentTetromino.position += XY{0, 1};
		}
	}
	if(::IsKeyPressed(KEY_SPACE))
	{
		for(int yOffset = 1;; ++yOffset)
		{
			if(playfield.overlapAt(currentTetromino.position + XY{0, yOffset}, currentTetromino.collision) != 0)
			{
				currentTetromino.position += XY{0, yOffset - 1};
				prevTetrominoOffset = currentTetromino;
				score += scoringSystem->process(ScoreEvent::HardDrop, yOffset - 1, levelState.currentLevel);
				isLocking = true;
				lockDelay.reset(lockDelay.delay);
				break;
			}
		}
	}

	if(!isLocking && playfield.overlapAt(currentTetromino.position + XY{0, 1}, currentTetromino.collision) != 0)
	{
		isLocking = true;
		lockDelay.reset();
	}
	if(isLocking && lockDelay.tick(::GetFrameTime()))
	{
		const ScoreEvent scoreEvent = tSpinFunc(playfield, currentTetromino, currentTetromino - prevTetrominoOffset);

		playfield.setAt(currentTetromino.position, currentTetromino.collision);

		const size_t linesCleared = eraseFullLines(playfield);
		score += scoringSystem->process(scoreEvent, static_cast<int>(linesCleared), levelState.currentLevel);
		levelState = levelUpFunc(scoreEvent, static_cast<int>(linesCleared), levelState);

		isLocking = false;
		currentTetromino = getNextTetromino(app.settings.previewCount);
		if(playfield.overlapAt(currentTetromino.position, currentTetromino.collision) != 0)
		{
			state = State::GameOver;
		}
	}
}

void Game::draw(App& app)
{
	::BeginDrawing();

	::ClearBackground(LIGHTGRAY);

	const raylib::Rectangle playfieldBorderBounds(static_cast<float>(playfieldBounds.x - FIELD_BORDER_WIDTH),
	    static_cast<float>(playfieldBounds.y - FIELD_BORDER_WIDTH),
	    static_cast<float>(playfieldBounds.width + (FIELD_BORDER_WIDTH * 2) - 1),
	    static_cast<float>(playfieldBounds.height + (FIELD_BORDER_WIDTH * 2) - 1));
	const int cellSize = (playfieldBounds.width / playfield.getSize().width) - 1;
	const XY hiddenOffset{0, (cellSize + 1) * HIDDEN_HEIGHT};
	drawBackground(playfield, playfieldBounds - hiddenOffset, cellSize, 1, LIGHTGRAY, DARKGRAY);
	drawCells(playfield, playfieldBounds - hiddenOffset, cellSize, 1, minoColors);
	::DrawRectangle(playfieldBounds.x, 0, playfieldBounds.width, static_cast<int>(playfieldBorderBounds.y), LIGHTGRAY);
	::DrawRectangleLinesEx(playfieldBorderBounds, FIELD_BORDER_WIDTH, DARKGRAY);

	drawCells(currentTetromino.collision,
	    ((currentTetromino.position - XY{0, HIDDEN_HEIGHT}) * (cellSize + 1)) + playfieldBounds, cellSize, 1,
	    minoColors);

	if(holdPieceIdx != -1)
	{
		drawCells(
		    baseTetrominos[holdPieceIdx].collision, previewOffsetsMain[holdPieceIdx], PREVIEW_CELL_SIZE, 1, minoColors);
	}

	if(app.settings.previewCount > 0)
	{
		drawCells(baseTetrominos[nextTetrominoIndices[0]].collision,
		    previewOffsetsMain[nextTetrominoIndices[0]] + XY{App::Settings::SCREEN_WIDTH - SIDEBAR_WIDTH, 0},
		    PREVIEW_CELL_SIZE, 1, minoColors);

		for(int i = 1; i < app.settings.previewCount; ++i)
		{
			drawCells(baseTetrominos[nextTetrominoIndices[i]].collision,
			    previewOffsetsExtended[nextTetrominoIndices[i]] +
			        XY{0, ((i - 1) * previewElementHeightExtended) + PREVIEW_ELEMENT_HEIGHT},
			    cellSizeExtended(), 1, minoColors);
		}
	}

	{
		std::array<char, 32> scoreBuffer{};
		char* scoreBufferBegin = scoreBuffer.data();
		char* scoreBufferEnd = scoreBuffer.data() + scoreBuffer.size();
		char* scoreBufferParsedEnd = std::to_chars(scoreBufferBegin, scoreBufferEnd, score).ptr;
		const ptrdiff_t length = std::distance(scoreBufferBegin, scoreBufferParsedEnd);
		const ptrdiff_t separators = (length - 1) / 3;
		char* scoreBufferWrite = scoreBufferParsedEnd + (separators - 1);
		char* scoreBufferRead = scoreBufferParsedEnd - 1;
		ptrdiff_t separatorsWritten = 0;
		while(separatorsWritten < separators)
		{
			for(int digitsRead = 0; digitsRead < 3; ++digitsRead)
			{
				*scoreBufferWrite = *scoreBufferRead;
				--scoreBufferWrite;
				--scoreBufferRead;
			}
			*scoreBufferWrite = '.';
			++separatorsWritten;
			--scoreBufferWrite;
		}
		const int scoreTextWidth = ::MeasureText(scoreBufferBegin, SCORE_FONT_SIZE);
		const int scoreTextXOffset = (SIDEBAR_WIDTH - scoreTextWidth) / 2;
		::DrawText(
		    scoreBufferBegin, scoreTextXOffset, PREVIEW_ELEMENT_HEIGHT + SCORE_FONT_SIZE, SCORE_FONT_SIZE, DARKGRAY);
	}

	if(state != State::Running)
	{
		const std::string_view statusText = state == State::Paused ? "Paused" : "Game Over";
		const float statusTextSpacing = STATUS_FONT_SIZE / 10.0f;
		const raylib::Vector2 statusTextSize =
		    ::MeasureTextEx(::GetFontDefault(), statusText.data(), STATUS_FONT_SIZE, statusTextSpacing);
		const raylib::Vector2 statusTextBackgroundSize =
		    statusTextSize + raylib::Vector2{statusTextSpacing * 2, statusTextSpacing * 2};
		const raylib::Vector2 centerPosition{App::Settings::SCREEN_WIDTH / 2.0f, App::Settings::SCREEN_HEIGHT / 2.0f};
		const raylib::Vector2 statusTextBackgroundPosition{centerPosition.x - (statusTextBackgroundSize.x / 2.0f),
		    centerPosition.y - (statusTextBackgroundSize.y / 2.0f)};
		::DrawRectangleRec({statusTextBackgroundPosition.x, statusTextBackgroundPosition.y, statusTextBackgroundSize.x,
		                       statusTextBackgroundSize.y},
		    STATUS_BACKGROUND);
		const raylib::Vector2 statusTextPosition{
		    centerPosition.x - (statusTextSize.x / 2), centerPosition.y - (statusTextSize.y / 2)};
		::DrawTextEx(::GetFontDefault(), statusText.data(), {statusTextPosition.x, statusTextPosition.y},
		    STATUS_FONT_SIZE, statusTextSpacing, RED);
	}

	::EndDrawing();
}

void Game::UpdateDraw(App& app)
{
	update(app);
	draw(app);
}

Game::Game(App& app) :
    playfield{{app.settings.fieldWidth, app.settings.fieldHeight + HIDDEN_HEIGHT}, 0},
    playfieldBounds{calculatePlayfieldBounds({app.settings.fieldWidth, app.settings.fieldHeight})},
    baseTetrominos{prepareTetrominos(makeBaseMinos(app.settings.rotationSystem)(), playfield.getSize().width)},
    previewOffsetsMain{calcCenterOffsets(baseTetrominos, {SIDEBAR_WIDTH, PREVIEW_ELEMENT_HEIGHT}, PREVIEW_CELL_SIZE)},
    holdPieceIdx{static_cast<size_t>(-1)}, rng{std::random_device{}()},
    shuffledIndicesFunc{shuffledIndices(app.settings.shuffleType)},
    nextTetrominoIndices{fillIndices({}, app.settings.previewCount)},
    previewElementHeightExtended{
        app.settings.previewCount < 2
            ? 0
            : (App::Settings::SCREEN_HEIGHT - PREVIEW_ELEMENT_HEIGHT) / (app.settings.previewCount - 1)},
    previewOffsetsExtended{
        calcCenterOffsetsExtended(baseTetrominos, {SIDEBAR_WIDTH, previewElementHeightExtended}, cellSizeExtended())},
    currentTetromino{getNextTetromino(app.settings.previewCount)},
    scoringSystem{makeScoringSystem(app.settings.scoringSystem)()}, score{0}, state{State::Running},
    levelUpFunc{levelUp(app.settings.levelGoal)}, levelState{LevelState::make(app.settings.levelGoal)},
    lockDelay{App::Settings::LOCK_DELAY}, isLocking{false}, tSpinFunc{tSpinCheck(app.settings.tSpin)},
    moveRight{App::Settings::DELAYED_AUTO_SHIFT, App::Settings::AUTO_REPEAT_RATE, KEY_RIGHT, KEY_LEFT}
{
}

std::deque<size_t> Game::fillIndices(std::deque<size_t> indices, size_t minIndices)
{
	while(indices.size() < minIndices)
	{
		std::vector<size_t> newIndices = shuffledIndicesFunc(baseTetrominos, rng);
		indices.insert(indices.end(), newIndices.begin(), newIndices.end());
	}
	return indices;
}

int Game::cellSizeExtended() const
{
	return std::min(previewElementHeightExtended / 5, PREVIEW_CELL_SIZE);
}

Tetromino Game::getNextTetromino(size_t minIndices)
{
	minIndices = minIndices == 0 ? 1 : minIndices;
	const size_t nextIdx = nextTetrominoIndices.front();
	nextTetrominoIndices.pop_front();
	nextTetrominoIndices = fillIndices(std::move(nextTetrominoIndices), minIndices);
	return baseTetrominos[nextIdx];
}

template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>([[maybe_unused]] App& app)
{
	return std::make_unique<Game>(app);
}
} // namespace raymino
