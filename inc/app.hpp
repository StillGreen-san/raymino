#pragma once

#include "scenes.hpp"
#include "types.hpp"

#include <memory>

namespace raymino
{
/**
 * @brief The Application, call Make() with the starting scene to create an instance and Run() to start the loop
 */
class App
{
public:
	struct Settings
	{
		RotationSystem rotationSystem = RotationSystem::Super;
		WallKicks wallKicks = WallKicks::Super;
		LockDown lockDown = LockDown::Extended;
		SoftDrop softDrop = SoftDrop::NonLocking;
		InstantDrop instantDrop = InstantDrop::Hard;
		TSpin tSpin = TSpin::ThreeCorner;
		ShuffleType shuffleType = ShuffleType::SingleBag;
		ScoringSystem scoringSystem = ScoringSystem::Guideline;
		LevelGoal levelGoal = LevelGoal::Fixed;
		bool holdPiece = true;
		bool ghostPiece = true;
		int fieldWidth = 10;
		int fieldHeight = 20;
		int previewCount = 6;
		bool operator==(const Settings& rhs) const;
		bool operator!=(const Settings& rhs) const;

		static constexpr int SCREEN_WIDTH = 600;
		static constexpr int SCREEN_HEIGHT = 600;
		static constexpr float LOCK_DELAY = 0.5f;
		static constexpr float DELAYED_AUTO_SHIFT = 1.0f / 6.0f;
		static constexpr float AUTO_REPEAT_RATE = 1.0f / 30.0f;
	};

	/**
	 * @brief create an instance (only one should exist)
	 * @tparam TScene starting scene enum
	 * @return App instance
	 */
	template<Scene TScene>
	static App Make()
	{
		App app;
		app.currentScene = MakeScene<TScene>(app);
		return app;
	}

	/**
	 * @brief start the main loop, calling IScene::UpdateDraw() repeatedly
	 */
	void Run();

	/**
	 * @brief calls UpdateDraw on currentScene
	 */
	void UpdateDraw();

	/**
	 * @param newScene to switch to
	 */
	void QueueSceneSwitch(std::unique_ptr<IScene> newScene);

	Settings settings;

private:
	App();

	std::unique_ptr<IScene> currentScene;
	std::unique_ptr<IScene> nextScene = nullptr;
};
} // namespace raymino
