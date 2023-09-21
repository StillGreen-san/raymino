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

		bool operator==(const KeyBinds& rhs) const noexcept;
		bool operator!=(const KeyBinds& rhs) const noexcept;
		bool operator>(const KeyBinds& rhs) const noexcept;
		bool operator<(const KeyBinds& rhs) const noexcept;
		[[nodiscard]] int compare(const KeyBinds& rhs) const noexcept;
	};

	struct alignas(int64_t) HighScoreEntry
	{
		using NameT = std::array<char, 8>;
		/**
		 * @return copied chars without ending \0
		 */
		static size_t copyInto(const char* inPtr, NameT& outRef) noexcept;
		HighScoreEntry(const char* namePtr, int64_t score, const Settings& settings) noexcept;
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
	 * @brief collection of named values with initial set of fixed items
	 * @tparam TValue type of Item value
	 */
	template<typename TValue = int>
	class Presets
	{
	public:
		using NameT = std::array<char, 16>;
		struct alignas(int64_t) Item
		{
			NameT name;
			TValue value;
			Item(const NameT& name, const TValue& value) noexcept(std::is_nothrow_copy_constructible_v<TValue>) :
			    name{name}, value{value}
			{
			}
			Item(std::string_view name, const TValue& value) noexcept(std::is_nothrow_copy_constructible_v<TValue>) :
			    name{}, value{value}
			{
				std::copy_n(name.begin(), std::min(name.size(), this->name.size() - 1), this->name.begin());
			}
			operator std::string_view() const noexcept
			{
				return {name.data()};
			}
		};

		/**
		 * @brief construct with optional set of fixed items
		 */
		Presets(std::initializer_list<Item> fixed) : items{fixed}
		{
		}

		/**
		 * @brief adds a range of items to the end of the collection
		 */
		template<typename TItem>
		void add(Range<TItem> itemRange)
		{
			items.insert(items.end(), itemRange.begin(), itemRange.end());
		}

		/**
		 * @brief add item to end of collection
		 */
		void add(Item item)
		{
			items.emplace_back(item);
		}

		/**
		 * @brief remove index if item is not fixed
		 */
		void remove(size_t index) noexcept
		{
			if(index >= fixed())
			{
				items.erase(items.begin() + index);
			}
		}

		/**
		 * @brief find Item index where value == item.value
		 * @param value to find
		 * @return index or size() if not found
		 */
		[[nodiscard]] size_t find(const TValue& value) const noexcept
		{
			const auto itemIt = std::find_if(items.begin(), items.end(),
			    [&](const Item& item)
			    {
				    return item.value == value;
			    });
			return std::distance(items.begin(), itemIt);
		}

		/**
		 * @brief find Item index where name == item.name
		 * @param name to find
		 * @return index or size() if not found
		 */
		[[nodiscard]] size_t find(std::string_view name) const noexcept
		{
			const auto itemIt = std::find_if(items.begin(), items.end(),
			    [&](const Item& item)
			    {
				    return std::string_view{item.name.data()} == name;
			    });
			return std::distance(items.begin(), itemIt);
		}

		[[nodiscard]] const std::vector<Item>& get() const noexcept
		{
			return items;
		}

		[[nodiscard]] const Item& get(size_t index) const noexcept
		{
			return items[index];
		}

		[[nodiscard]] Item* tryGet(size_t index) noexcept
		{
			return index >= fixed() ? &items[index] : nullptr;
		}

		/**
		 * @return total number of items
		 */
		[[nodiscard]] size_t size() const noexcept
		{
			return items.size();
		}

		/**
		 * @return number of immutable/fixed items
		 */
		[[nodiscard]] size_t fixed() const noexcept
		{
			return fixedItems;
		}

	private:
		std::vector<Item> items;
		size_t fixedItems{items.size()};
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
	 * @param scene to switch to
	 */
	void QueueSceneSwitch(Scene scene);

	/**
	 * @return true if score == highest for namePtr+settings
	 */
	bool addHighScore(int64_t score);

	HighScoreEntry::NameT playerName;
	Presets<KeyBinds> keyBindsPresets;
	Presets<Settings> settingsPresets;
	uint32_t activeKeyBindsPreset;
	uint32_t activeSettingsPreset;
	HighScores highScores;
	std::array<char, 20> seed;

	/**
	 * @brief return active KeyBinds preset
	 */
	[[nodiscard]] const KeyBinds& keyBinds() const noexcept;

	/**
	 * @brief return active Settings preset
	 */
	[[nodiscard]] const Settings& settings() const noexcept;

	static constexpr const char* FILE_PATH = "save.raymino";
	static constexpr const char* IDB_PATH = "raymino";
	static constexpr size_t MAX_PRESETS = std::numeric_limits<uint16_t>::max();
#if defined(PLATFORM_WEB)
	static constexpr size_t MAX_SCORES = 1300;
#else
	static constexpr size_t MAX_SCORES = 5000;
#endif

	static_assert(sizeof(bool) == 1);
	static_assert(sizeof(Settings) == 16);
	static_assert(sizeof(KeyBinds) == 24);
	static_assert(sizeof(HighScoreEntry) == 32);
	static_assert(sizeof(Presets<KeyBinds>::Item) == 40);
	static_assert(sizeof(Presets<Settings>::Item) == 32);
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
