#pragma once

#include "savefile.hpp"
#include "scenes.hpp"
#include "types.hpp"

#include <raylib.h>

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
	struct alignas(int64_t) Settings
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
		[[maybe_unused]] uint8_t _reserved_[2]{}; // NOLINT(*-avoid-c-arrays)

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
	struct alignas(int64_t) KeyBinds
	{
		int16_t moveRight = KEY_RIGHT;
		int16_t moveLeft = KEY_LEFT;
		int16_t rotateRight = KEY_V;
		int16_t rotateLeft = KEY_X;
		int16_t softDrop = KEY_DOWN;
		int16_t hardDrop = KEY_SPACE;
		int16_t hold = KEY_C;
		int16_t pause = KEY_ESCAPE;
		int16_t restart = KEY_HOME;
		int16_t menu = KEY_END;
		[[maybe_unused]] int16_t _reserved_[2]{}; // NOLINT(*-avoid-c-arrays)
	};

	struct HighScoreEntry
	{
		using NameT = std::array<char, 8>;
		/**
		 * @return copied chars without ending \0
		 */
		static size_t copyInto(const char* inPtr, NameT& outRef);
		HighScoreEntry(const char* namePtr, int64_t score, const Settings& settings);
		NameT name;
		int64_t score;
		Settings settings;
	};
	struct HighScores
	{
		std::vector<HighScoreEntry> entries;
		/**
		 * @return true if score == highest for namePtr+settings
		 */
		bool add(const char* namePtr, int64_t score, const Settings& settings);
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
	bool addHighScore(int64_t score);

	HighScoreEntry::NameT playerName;
	Settings settings;
	KeyBinds keyBinds;
	HighScores highScores;

	static constexpr const char* FILE_PATH = "save.raymino";
	static constexpr const char* IDB_PATH = "raymino";
#if defined(PLATFORM_WEB)
	static constexpr size_t MAX_SCORES = 1300;
#else
	static constexpr size_t MAX_SCORES = 5000;
#endif

	static_assert(sizeof(bool) == 1);
	static_assert(sizeof(Settings) == 16);
	static_assert(sizeof(KeyBinds) == 24);
	static_assert(sizeof(HighScoreEntry) == 32);
	static_assert(sizeof(decltype(HighScoreEntry::score)) == 8);
	static_assert(__STDCPP_DEFAULT_NEW_ALIGNMENT__ % 8 == 0);

	/**
	 * @brief attempts to create SaveFile from compressed data
	 * @param compressedData sdefl compressed data with uncompressed Header
	 * @param size of data
	 * @return SaveFile empty if verification failed
	 */
	static SaveFile decompressFile(const void* compressedData, uint32_t size);

	/**
	 * @brief saves SaveFile to disc, compressing with sinfl
	 * @param save SaveFile
	 */
	static void storeFile(const SaveFile& save);

	[[nodiscard]] SaveFile serialize() const;
	void deserialize(const SaveFile& save);

private:
	std::unique_ptr<IScene> currentScene;
	std::unique_ptr<IScene> nextScene = nullptr;
};
} // namespace raymino
