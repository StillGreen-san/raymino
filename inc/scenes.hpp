#pragma once

#include <memory>

namespace raymino
{
struct IScene
{
	virtual void UpdateDraw(struct App& app) = 0;
	virtual ~IScene() = default;
};

enum class Scene
{
	Cellular
};

template<Scene TScene>
std::unique_ptr<IScene> MakeScene();
} // namespace raymino
