#pragma once

#include <memory>

namespace raymino
{
/**
 * @brief scene interface to render in App, derive from this add to Scene enum & implement MakeScene
 */
struct IScene
{
	virtual void UpdateDraw(class App& app) = 0;
	virtual ~IScene() = default;
};

/**
 * @brief scene identification, add entry, derive from IScene & implement MakeScene
 */
enum class Scene
{
	Cellular,
	Game,
};

/**
 * @brief scene creation, implement, derive from IScene & add entry to Scene
 * @tparam TScene Scene enum
 * @return std::unique_ptr<IScene> of TScene type
 */
template<Scene TScene>
std::unique_ptr<IScene> MakeScene();
} // namespace raymino
