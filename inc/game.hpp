#include "app.hpp"
#include "grid.hpp"
#include "playfield.hpp"
#include "scenes.hpp"
#include "timer.hpp"
#include "types.hpp"

#include <vector>

#define HIDDEN_HEIGHT 4

namespace raymino
{
enum class State
{
	Drop,
	Set,
	Over,
};

void shuffleBaseMinos(const std::vector<Grid>& baseMinos,
    std::back_insert_iterator<std::vector<Grid>> nextMinosInserter, size_t minCount);
XY getStartPosition(const Grid& mino, unsigned fieldWidth);

struct Game : IScene
{
	void dropMino();
	void moveMino(float delta);
	void rotateMino(float delta);
	void setMino();
	void update(App& app);
	void drawPlayfield();
	void draw();
	void UpdateDraw(App& app) override;
	Playfield playfield{{10, 20 + HIDDEN_HEIGHT},
	    {{{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}, {{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}},
	        {{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}, {{2, 2}, {1, 1, 1, 1}}, {{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}},
	        {{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}}, {{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}},
	    shuffleBaseMinos, getStartPosition};
	State state{State::Drop};
	Timer dropDelay{};
	Timer moveDelay{1.f/3.f};
	Timer rotateDelay{1.f/3.f};
};
} // namespace raymino
