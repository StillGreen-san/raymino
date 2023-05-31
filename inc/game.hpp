#include "app.hpp"
#include "gameplay.hpp"
#include "grid.hpp"
#include "input.hpp"
#include "scenes.hpp"
#include "timer.hpp"
#include "types.hpp"

#include <vector>

namespace raymino
{
struct Game : IScene
{
	Game(App& app);
	void UpdateDraw(App& app) override;
	void update(App& app);
	void draw();

	Grid playfield;
	Rect playfieldBounds;
};
} // namespace raymino
