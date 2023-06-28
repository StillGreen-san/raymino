#pragma once

#include "scenes.hpp"
#include "types.hpp"

#include <array>
#include <memory>
#include <vector>

namespace raymino
{
/**
 * @brief The Application, starts in Loading Scene, call Run() to start the loop
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
		uint8_t fieldWidth = 10;
		uint8_t fieldHeight = 20;
		uint8_t previewCount = 6;
		bool operator==(const Settings& rhs) const noexcept;
		bool operator!=(const Settings& rhs) const noexcept;
		bool operator>(const Settings& rhs) const noexcept;
		bool operator<(const Settings& rhs) const noexcept;
		[[nodiscard]] int compare(const Settings& rhs) const noexcept;

		static constexpr int SCREEN_WIDTH = 600;
		static constexpr int SCREEN_HEIGHT = 600;
		static constexpr float LOCK_DELAY = 0.5f;
		static constexpr float DELAYED_AUTO_SHIFT = 1.0f / 6.0f;
		static constexpr float AUTO_REPEAT_RATE = 1.0f / 30.0f;
	};

	struct HighScoreEntry
	{
		using NameT = std::array<char, 8>;
		/**
		 * @return copied chars without ending \0
		 */
		static size_t copyInto(const char* inPtr, NameT& outRef);
		HighScoreEntry(const char* namePtr, ptrdiff_t score, const Settings& settings);
		NameT name;
		ptrdiff_t score;
		Settings settings;
	};
	struct HighScores
	{
		std::vector<HighScoreEntry> entries;
		/**
		 * @return true if score == highest for namePtr+settings
		 */
		bool add(const char* namePtr, ptrdiff_t score, const Settings& settings);
	};

	/**
	 * @brief create an instance (only one should exist)
	 */
	App();

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

	/**
	 * @return true if score == highest for namePtr+settings
	 */
	bool addHighScore(ptrdiff_t score);

	HighScoreEntry::NameT playerName;
	Settings settings;
	HighScores highScores;

	static constexpr size_t FILE_VERSION = 0;
	static constexpr const char* FILE_PATH = "save.raymino";
	static constexpr const char* IDB_PATH = "raymino";
#if defined(PLATFORM_WEB)
	static constexpr size_t MAX_SCORES = 200;
#else
	static constexpr size_t MAX_SCORES = 5000;
#endif

	std::vector<unsigned char> serialize();
	void deserialize(unsigned char* data, unsigned bytes);

private:
	std::unique_ptr<IScene> currentScene;
	std::unique_ptr<IScene> nextScene = nullptr;
};
} // namespace raymino
