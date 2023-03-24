#include "app.hpp"
#include "grid.hpp"
#include "scenes.hpp"
#include "types.hpp"

#include <vector>

namespace raymino
{
enum class State
{
	Drop,
	Set,
	Over,
};
struct ActiveMino
{
	ActiveMino() = delete;
	ActiveMino(const Grid& mino, XY position, uint8_t color);
	Grid color;
	Grid collision;
	XY position;
};

struct Game : IScene
{
	void dropMino();
	void setMino();
	void update(App& app);
	void draw();
	void UpdateDraw(App& app) override;
	std::vector<Grid> shuffledBaseMinos();
	ActiveMino takeNextMino();
	XY getStartPosition(const Grid& mino);
	Grid playArea{{10, 20 + 4}, 0};
	std::vector<Grid> baseMinos{{{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
	    {{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}}, {{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}, {{2, 2}, {1, 1, 1, 1}},
	    {{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}}, {{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}},
	    {{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}};
	std::vector<Grid> nextMinos{shuffledBaseMinos()};
	ActiveMino activeMino{takeNextMino()};
	State state{State::Drop};
};
} // namespace raymino
