#include "app.hpp"
#include "gameplay.hpp"
#include "grid.hpp"
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
	Game(App& app);
	void UpdateDraw(App& app) override;

	void update(App& app);
	void draw(App& app);

	std::deque<size_t> fillIndices(std::deque<size_t> indices, size_t minIndices);
	int cellSizeExtended() const;
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
	std::default_random_engine rng;
	decltype(shuffledIndices(ShuffleType{})) shuffledIndicesFunc;
	std::deque<size_t> nextTetrominoIndices;
	int previewElementHeightExtended;
	std::vector<XY> previewOffsetsExtended;
	Tetromino currentTetromino;
	std::unique_ptr<IScoringSystem> scoringSystem;
	ptrdiff_t score;
	State state;
	decltype(levelUp(LevelGoal{})) levelUpFunc;
	LevelState levelState;
	Timer lockDelay;
	bool isLocking;
	decltype(tSpinCheck(TSpin{})) tSpinFunc;
	Timer gravity;
};
} // namespace raymino
