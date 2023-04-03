#include "app.hpp"
#include "grid.hpp"
#include "input.hpp"
#include "playfield.hpp"
#include "scenes.hpp"
#include "timer.hpp"
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

struct Game : IScene
{
	Game(Playfield playfield, State state, Timer dropDelay, KeyAction drop, KeyAction move, KeyAction rotate, size_t score);
	void dropMino();
	void moveMino(float delta);
	void rotateMino(float delta);
	void setMino();
	void update(App& app);
	void draw();
	void UpdateDraw(App& app) override;
	Playfield playfield;
	State state;
	Timer dropDelay;
	KeyAction drop;
	KeyAction move;
	KeyAction rotate;
	size_t score;
};
} // namespace raymino
