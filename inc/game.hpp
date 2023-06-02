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
	void draw();

	void fillIndices(size_t minIndices);

	Grid playfield;
	Rect playfieldBounds;
	std::vector<Tetromino> baseTetrominos;
	std::vector<XY> previewOffsetsMain;
	size_t holdPieceIdx;
	std::default_random_engine rng;
	std::vector<size_t> (*shuffledIndicesFunc)(const std::vector<Tetromino>&, std::default_random_engine&);
	std::deque<size_t> previewTetrominoIndices;
};
} // namespace raymino
