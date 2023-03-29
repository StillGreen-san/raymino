#include "app.hpp"
#include "grid.hpp"
#include "playfield.hpp"
#include "scenes.hpp"
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
	void moveMino();
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
	float time = 0;
	float delay = 0.1666f;
};
} // namespace raymino
