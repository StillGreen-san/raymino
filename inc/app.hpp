#pragma once

#include "scenes.hpp"

#include <memory>

namespace raymino
{
/**
 * @brief The Application, call Make() with the starting scene to create an instance and Run() to start the loop
 */
class App
{
public:
	/**
	 * @brief create an instance (only one should exist)
	 * @tparam TScene starting scene enum
	 * @return App instance
	 */
	template<Scene TScene>
	static App Make()
	{
		App app;
		app.currentScene = MakeScene<TScene>();
		return app;
	}

	/**
	 * @brief start the main loop, calling IScene::UpdateDraw() repeatedly
	 */
	void Run();

	std::unique_ptr<IScene> currentScene;
private:
	App();
};
} // namespace raymino
