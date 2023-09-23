#include "game.hpp"

#include "app.hpp"
#include "gameplay.hpp"
#include "graphics.hpp"
#include "grid.hpp"
#include "gui.hpp"
#include "scenes.hpp"
#include "timer.hpp"
#include "types.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

bool constexpr operator<(::Color lhs, ::Color rhs) noexcept
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
template<>
std::unique_ptr<IScene> MakeScene<Scene::Game>(App& app)
{
	return std::make_unique<Game>(app);
}

void Game::PreDestruct([[maybe_unused]] App& app)
{
}

constexpr int HIDDEN_HEIGHT = 4;
constexpr int SIDEBAR_WIDTH = 150;
constexpr int PREVIEW_ELEMENT_HEIGHT = 100;
constexpr int PREVIEW_CELL_SIZE = 30;
constexpr int FIELD_BORDER_WIDTH = 2;
constexpr XY OFFSCREEN_POSITION{-1337, -1337};
constexpr int SCORE_FONT_SIZE = 30;
constexpr int STATUS_FONT_SIZE = 50;
constexpr ::Color STATUS_BACKGROUND{77, 77, 77, 222};
constexpr int LOCKDOWN_MAX_RESET = 15;
constexpr size_t NO_HOLD_PIECE = std::numeric_limits<size_t>::max();

const ColorMap minoColors{
    {LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE,
        DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN, WHITE, BLACK, BLANK, MAGENTA, RAYWHITE}};

void prepareTetromino(Tetromino& tetromino, Grid::Cell color, int fieldWidth) noexcept
{
	tetromino.collision.transformCells(
	    [color](Grid::Cell current)
	    {
		    return static_cast<Grid::Cell>(current * color);
	    });
	tetromino.position = spawnPosition(tetromino, HIDDEN_HEIGHT - 2, fieldWidth);
}

std::vector<Tetromino> prepareTetrominos(std::vector<Tetromino> tetrominos, int fieldWidth) noexcept
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

Rect calculatePlayfieldBounds(Size fieldSize) noexcept
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

	return {{xOffset, yOffset}, {actualWidth, actualHeight}};
}

XY calcCenterOffset(const Grid& grid, Size available, int cellSize) noexcept
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

bool isKeyPress(KeyAction::Return keyPress) noexcept
{
	return keyPress.value != 0 && keyPress.state != KeyAction::State::Released;
}

void Game::update(App& app)
{
	const App::KeyBinds& keyBinds = app.keyBinds();
	const App::Settings& settings = app.settings();

	if(::IsKeyPressed(keyBinds.menu))
	{
		app.QueueSceneSwitch(Scene::Menu);
		state = State::GameOver;
	}
	if(::IsKeyPressed(keyBinds.restart))
	{
		app.QueueSceneSwitch(Scene::Game);
	}
	if(::IsKeyPressed(keyBinds.pause))
	{
		switch(state)
		{
		case State::Running:
			state = State::Paused;
			break;
		case State::Paused:
			state = State::Running;
			break;
		case State::GameOver:
			app.QueueSceneSwitch(Scene::Game);
			break;
		}
	}
	if(state == State::Running && !::IsWindowFocused())
	{
		state = State::Paused;
	}

	if(state != State::Running)
	{
		return;
	}

	if(settings.holdPiece && !holdPieceLocked && ::IsKeyPressed(keyBinds.hold))
	{
		if(holdPieceIdx == NO_HOLD_PIECE)
		{
			holdPieceIdx = static_cast<size_t>(currentTetromino.type);
			currentTetromino = getNextTetromino(settings.previewCount);
		}
		else
		{
			const auto nextHoldIdx = static_cast<size_t>(currentTetromino.type);
			currentTetromino = baseTetrominos[holdPieceIdx];
			holdPieceIdx = nextHoldIdx;
		}
		isLocking = false;
		lockCounter = 0;
		holdPieceLocked = true;
	}

	Offset prevTetrominoOffset = currentTetromino;

	gravity.delay =
	    delays[std::min<size_t>((::IsKeyDown(keyBinds.softDrop) ? 2 : 0) + levelState.currentLevel, maxSpeedLevel)];

	if(const KeyAction::Return moveAction = moveRight.tick(::GetFrameTime()); isKeyPress(moveAction))
	{
		if(playfield.overlapAt(currentTetromino.position + XY{moveAction.value, 0}, currentTetromino.collision) == 0)
		{
			currentTetromino.position += XY{moveAction.value, 0};
			if(isLocking && settings.lockDown <= LockDown::Extended)
			{
				if(settings.lockDown == LockDown::Infinit || lockCounter < LOCKDOWN_MAX_RESET)
				{
					lockCounter += 1;
					lockDelay.reset(0);
				}
			}
		}
	}
	if(const KeyAction::Return rotateAction = rotateRight.tick(::GetFrameTime()); isKeyPress(rotateAction))
	{
		Offset rotation = basicRotationFunc(currentTetromino, rotateAction.value);
		currentTetromino += rotation;
		if(playfield.overlapAt(currentTetromino.position, currentTetromino.collision) != 0)
		{
			currentTetromino -= rotation;
			rotation = wallKickFunc(playfield, currentTetromino, rotation);
			currentTetromino += rotation;
		}
		if(isLocking && settings.lockDown <= LockDown::Extended && rotation != Offset{})
		{
			if(settings.lockDown == LockDown::Infinit || lockCounter < LOCKDOWN_MAX_RESET)
			{
				lockCounter += 1;
				lockDelay.reset(0);
			}
		}
	}
	if(gravity.step(::GetFrameTime()))
	{
		if(playfield.overlapAt(currentTetromino.position + XY{0, 1}, currentTetromino.collision) == 0)
		{
			currentTetromino.position += XY{0, 1};
			if(::IsKeyDown(keyBinds.softDrop))
			{
				score += scoringSystem->process(ScoreEvent::SoftDrop, 1, levelState.currentLevel);
			}
			if(isLocking)
			{
				switch(settings.lockDown)
				{
				case LockDown::Infinit:
					lockDelay.reset(0);
					break;
				case LockDown::Extended:
					if(lockCounter < LOCKDOWN_MAX_RESET)
					{
						lockCounter += 1;
						lockDelay.reset(0);
					}
					break;
				case LockDown::Classic:
					lockDelay.reset(0);
					break;
				case LockDown::Entry:
				case LockDown::Instant:
					break;
				}
			}
		}
		else if(::IsKeyDown(keyBinds.softDrop) && settings.softDrop == SoftDrop::Locking)
		{
			isLocking = true;
			lockDelay.reset(lockDelay.delay);
		}
	}
	if(settings.instantDrop != InstantDrop::None && ::IsKeyPressed(keyBinds.hardDrop))
	{
		for(int yOffset = 1;; ++yOffset)
		{
			if(playfield.overlapAt(currentTetromino.position + XY{0, yOffset}, currentTetromino.collision) != 0)
			{
				currentTetromino.position += XY{0, yOffset - 1};
				prevTetrominoOffset = currentTetromino;
				score += scoringSystem->process(ScoreEvent::HardDrop, yOffset - 1, levelState.currentLevel);
				if(settings.instantDrop == InstantDrop::Hard)
				{
					isLocking = true;
					lockDelay.reset(lockDelay.delay);
				}
				break;
			}
		}
	}

	const bool onGround = playfield.overlapAt(currentTetromino.position + XY{0, 1}, currentTetromino.collision) != 0;

	if(!isLocking && onGround)
	{
		isLocking = true;
		lockDelay.reset(0);
	}
	if(isLocking && !onGround && settings.lockDown == LockDown::Entry)
	{
		lockDelay.tick(::GetFrameTime());
	}
	if(isLocking && onGround && lockDelay.tick(::GetFrameTime()))
	{
		const ScoreEvent scoreEvent = tSpinFunc(playfield, currentTetromino, currentTetromino - prevTetrominoOffset);

		playfield.setAt(currentTetromino.position, currentTetromino.collision);

		const size_t linesCleared = eraseFullLines(playfield);
		score += scoringSystem->process(scoreEvent, static_cast<int>(linesCleared), levelState.currentLevel);
		levelState = levelUpFunc(scoreEvent, static_cast<int>(linesCleared), levelState);
		if(isEmpty(playfield))
		{
			score += scoringSystem->process(
			    ScoreEvent::PerfectClear, static_cast<int>(linesCleared), levelState.currentLevel);
		}

		isLocking = false;
		holdPieceLocked = false;
		lockCounter = 0;
		currentTetromino = getNextTetromino(settings.previewCount);
		if(playfield.overlapAt(currentTetromino.position, currentTetromino.collision) != 0)
		{
			state = State::GameOver;
			isHighScore = app.addHighScore(score.value());
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

	const App::Settings& settings = app.settings();

	if(settings.ghostPiece)
	{
		int yOffset = 1;
		while(playfield.overlapAt(currentTetromino.position + XY{0, yOffset}, currentTetromino.collision) == 0)
		{
			++yOffset;
		}
		--yOffset;

		drawCells(currentTetromino.collision,
		    ((currentTetromino.position - XY{0, HIDDEN_HEIGHT - yOffset}) * (cellSize + 1)) + playfieldBounds, cellSize,
		    1, minoColors, 96);
	}

	drawCells(currentTetromino.collision,
	    ((currentTetromino.position - XY{0, HIDDEN_HEIGHT}) * (cellSize + 1)) + playfieldBounds, cellSize, 1,
	    minoColors);

	::DrawRectangle(playfieldBounds.x, 0, playfieldBounds.width, static_cast<int>(playfieldBorderBounds.y), LIGHTGRAY);
	::DrawRectangleLinesEx(playfieldBorderBounds, FIELD_BORDER_WIDTH, DARKGRAY);

	if(holdPieceIdx != NO_HOLD_PIECE)
	{
		drawCells(
		    baseTetrominos[holdPieceIdx].collision, previewOffsetsMain[holdPieceIdx], PREVIEW_CELL_SIZE, 1, minoColors);
	}

	if(settings.previewCount > 0)
	{
		drawCells(baseTetrominos[nextTetrominoIndices[0]].collision,
		    previewOffsetsMain[nextTetrominoIndices[0]] + XY{App::Settings::SCREEN_WIDTH - SIDEBAR_WIDTH, 0},
		    PREVIEW_CELL_SIZE, 1, minoColors);

		for(int i = 1; i < settings.previewCount; ++i)
		{
			drawCells(baseTetrominos[nextTetrominoIndices[i]].collision,
			    previewOffsetsExtended[nextTetrominoIndices[i]] +
			        XY{0, ((i - 1) * previewElementHeightExtended) + PREVIEW_ELEMENT_HEIGHT},
			    cellSizeExtended(), 1, minoColors);
		}
	}

	{
		const int scoreTextWidth = ::MeasureText(score.c_str(), SCORE_FONT_SIZE);
		const int scoreTextXOffset = (SIDEBAR_WIDTH - scoreTextWidth) / 2;
		::DrawText(
		    score.c_str(), scoreTextXOffset, PREVIEW_ELEMENT_HEIGHT + SCORE_FONT_SIZE, SCORE_FONT_SIZE, DARKGRAY);
	}

	if(state != State::Running)
	{
		const std::string_view statusText = state == State::Paused ? "Paused"
		                                    : isHighScore          ? "HighScore\nGame Over"
		                                                           : "Game Over";
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

template<typename TContainer>
size_t hashSeedString(const TContainer& container)
{
	const size_t seedLen = std::strlen(container.data());
	if(seedLen > 0)
	{
		return std::hash<std::string_view>{}({container.data(), seedLen});
	}
	return std::hash<std::random_device::result_type>{}(std::random_device{}());
}

Game::Game(App& app) :
    playfield{{app.settings().fieldWidth, app.settings().fieldHeight + HIDDEN_HEIGHT}, 0},
    playfieldBounds{calculatePlayfieldBounds({app.settings().fieldWidth, app.settings().fieldHeight})},
    baseTetrominos{prepareTetrominos(makeBaseMinos(app.settings().rotationSystem)(), playfield.getSize().width)},
    previewOffsetsMain{calcCenterOffsets(baseTetrominos, {SIDEBAR_WIDTH, PREVIEW_ELEMENT_HEIGHT}, PREVIEW_CELL_SIZE)},
    holdPieceIdx{NO_HOLD_PIECE}, rng{hashSeedString(app.seed)},
    shuffledIndicesFunc{makeShuffledIndices(app.settings().shuffleType)(baseTetrominos)},
    nextTetrominoIndices{fillIndices(app.settings().previewCount)},
    previewElementHeightExtended{
        app.settings().previewCount < 2
            ? 0
            : (App::Settings::SCREEN_HEIGHT - PREVIEW_ELEMENT_HEIGHT) / (app.settings().previewCount - 1)},
    previewOffsetsExtended{
        calcCenterOffsetsExtended(baseTetrominos, {SIDEBAR_WIDTH, previewElementHeightExtended}, cellSizeExtended())},
    currentTetromino{getNextTetromino(app.settings().previewCount)},
    scoringSystem{makeScoringSystem(app.settings().scoringSystem)()}, score{0}, state{State::Running},
    levelUpFunc{levelUp(app.settings().levelGoal)}, levelState{LevelState::make(app.settings().levelGoal)},
    lockDelay{app.settings().lockDown == LockDown::Instant ? 0 : App::Settings::LOCK_DELAY}, lockCounter{0},
    isLocking{false}, holdPieceLocked{false}, isHighScore{false}, tSpinFunc{tSpinCheck(app.settings().tSpin)},
    moveRight{App::Settings::DELAYED_AUTO_SHIFT, App::Settings::AUTO_REPEAT_RATE, app.keyBinds().moveRight,
        app.keyBinds().moveLeft},
    basicRotationFunc{basicRotation(app.settings().rotationSystem)}, wallKickFunc{wallKick(app.settings().wallKicks)},
    rotateRight{App::Settings::DELAYED_AUTO_SHIFT, App::Settings::AUTO_REPEAT_RATE, app.keyBinds().rotateRight,
        app.keyBinds().rotateLeft}
{
}

std::deque<size_t> Game::fillIndices(size_t minIndices)
{
	std::deque<size_t> indices;
	shuffledIndicesFunc->fill(indices, minIndices, rng);
	return indices;
}

int Game::cellSizeExtended() const noexcept
{
	return std::min(previewElementHeightExtended / 5, PREVIEW_CELL_SIZE);
}

Tetromino Game::getNextTetromino(size_t minIndices)
{
	minIndices = minIndices == 0 ? 1 : minIndices;
	const size_t nextIdx = nextTetrominoIndices.front();
	nextTetrominoIndices.pop_front();
	shuffledIndicesFunc->fill(nextTetrominoIndices, minIndices, rng);
	return baseTetrominos[nextIdx];
}
} // namespace raymino
