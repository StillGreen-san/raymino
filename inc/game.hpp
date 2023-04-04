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
struct Game : IScene
{
	enum class State
	{
		Drop,
		Lock,
		Set,
		Over,
	};
	Game(Playfield playfield, State state,Timer dropDelay, Timer lockDelay, KeyAction drop, KeyAction move, KeyAction rotate, size_t score);
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
	Timer lockDelay;
	KeyAction drop;
	KeyAction move;
	KeyAction rotate;
	size_t score;
};
} // namespace raymino
