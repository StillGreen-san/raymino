#include "app.hpp"
#include "gameplay.hpp"
#include "grid.hpp"
#include "gui.hpp"
#include "input.hpp"
#include "scenes.hpp"
#include "timer.hpp"
#include "types.hpp"

#include <deque>
#include <random>
#include <vector>

namespace raymino
{
struct Game : IScene
{
	explicit Game(App& app);
	void UpdateDraw(App& app) override;
	void PreDestruct(App& app) override;

	void update(App& app);
	void draw(App& app);

	std::deque<size_t> fillIndices(size_t minIndices);
	[[nodiscard]] int cellSizeExtended() const noexcept;
	Tetromino getNextTetromino(size_t minIndices);

	enum class State
	{
		Running,
		Paused,
		GameOver,
	};

	Grid playfield;
	Rect playfieldBounds;
	std::vector<Tetromino> baseTetrominos;
	std::vector<XY> previewOffsetsMain;
	size_t holdPieceIdx;
	std::mt19937_64 rng;
	std::unique_ptr<IShuffledIndices> shuffledIndicesFunc;
	std::deque<size_t> nextTetrominoIndices;
	int previewElementHeightExtended;
	std::vector<XY> previewOffsetsExtended;
	Tetromino currentTetromino;
	std::unique_ptr<IScoringSystem> scoringSystem;
	NumberBuffer score;
	State state;
	decltype(levelUp(LevelGoal{})) levelUpFunc;
	LevelState levelState;
	Timer lockDelay;
	int lockCounter;
	bool isLocking;
	bool holdPieceLocked;
	bool isHighScore;
	decltype(tSpinCheck(TSpin{})) tSpinFunc;
	Timer gravity;
	KeyAction moveRight;
	decltype(basicRotation(RotationSystem{})) basicRotationFunc;
	decltype(wallKick(WallKicks{})) wallKickFunc;
	KeyAction rotateRight;
};
} // namespace raymino
