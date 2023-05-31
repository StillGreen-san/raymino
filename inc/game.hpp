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
	void UpdateDraw(App& app) override;
	void update(App& app);
	void draw();
};
} // namespace raymino
