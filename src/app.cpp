#include "app.hpp"

#include <external/sdefl.h>
#include <external/sinfl.h>
#include <raylib-cpp.hpp>

#include <magic_enum.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace raymino
{
namespace presets
{
App::Presets<App::Settings>::Item SettingsNES()
{
	App::Settings settings{};
	settings.rotationSystem = RotationSystem::NintendoRight;
	settings.wallKicks = WallKicks::None;
	settings.lockDown = LockDown::Instant;
	settings.softDrop = SoftDrop::Locking;
	settings.instantDrop = InstantDrop::None;
	settings.tSpin = TSpin::None;
	settings.shuffleType = ShuffleType::NES;
	settings.scoringSystem = ScoringSystem::Nintendo;
	settings.levelGoal = LevelGoal::Fixed;
	settings.holdPiece = false;
	settings.ghostPiece = false;
	settings.fieldWidth = 10;
	settings.fieldHeight = 20;
	settings.previewCount = 1;
	return {"Nintendo NES", settings};
}
App::Presets<App::Settings>::Item SettingsTGMLike()
{
	App::Settings settings{};
	settings.rotationSystem = RotationSystem::Arika;
	settings.wallKicks = WallKicks::Arika;
	settings.lockDown = LockDown::Classic;
	settings.softDrop = SoftDrop::Locking;
	settings.instantDrop = InstantDrop::Sonic;
	settings.tSpin = TSpin::Lenient;
	settings.shuffleType = ShuffleType::TGM35;
	settings.scoringSystem = ScoringSystem::Guideline;
	settings.levelGoal = LevelGoal::Fixed;
	settings.holdPiece = false;
	settings.ghostPiece = true;
	settings.fieldWidth = 10;
	settings.fieldHeight = 20;
	settings.previewCount = 1;
	return {"TGM Like", settings};
}
App::Presets<App::Settings>::Item SettingsGuideline()
{
	return {"Guideline", {}};
}
} // namespace presets

App::App() :
    playerName{"Mino"}, keyBindsPresets{{"Default", {}}},
    settingsPresets{{presets::SettingsGuideline(), presets::SettingsNES(), presets::SettingsTGMLike()}},
    activeKeyBindsPreset{0}, activeSettingsPreset{0}, seed{}
{
	::InitWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, "raymino");
	::SetWindowState(FLAG_VSYNC_HINT);
	::SetExitKey(KEY_NULL);
	currentScene = MakeScene<Scene::Loading>(*this);
}

void UpdateDraw(void* app)
{
	App& self = *static_cast<App*>(app);
	self.UpdateDraw();
}

void App::UpdateDraw()
{
	if(nextScene)
	{
		currentScene = std::move(nextScene);
	}
	currentScene->UpdateDraw(*this);
}

void App::Run()
{
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(raymino::UpdateDraw, this, 0, 1);
#else
	while(!::WindowShouldClose())
	{
		raymino::UpdateDraw(this);
	}
#endif
	currentScene->PreDestruct(*this);
	storeFile(serialize());
}

void App::QueueSceneSwitch(Scene scene)
{
	if(currentScene)
	{
		currentScene->PreDestruct(*this);
		storeFile(serialize());
	}
	switch(scene)
	{
	case Scene::Game:
		nextScene = MakeScene<Scene::Game>(*this);
		break;
	case Scene::Menu:
		nextScene = MakeScene<Scene::Menu>(*this);
		break;
	case Scene::Loading:
		nextScene = MakeScene<Scene::Loading>(*this);
		break;
	}
}

bool App::addHighScore(int64_t score)
{
	const bool isHighScore = highScores.add(playerName.data(), score, settingsPresets.get(activeSettingsPreset).value);
	if(highScores.entries.size() > MAX_SCORES)
	{
		highScores.entries.erase(next(begin(highScores.entries), MAX_SCORES), end(highScores.entries));
	}
	App::storeFile(serialize());
	return isHighScore;
}

struct ChunkType
{
	enum : uint16_t
	{
		PlayerName = 10,
		Settings = 11,
		HighScores = 12,
		KeyBinds = 13,
		KeyBindsPresets = 14,
		SettingsPresets = 15,
		OtherItems = 16,
	};
	using type = decltype(PlayerName);
};

struct alignas(int64_t) OtherItems
{
	[[deprecated]] uint32_t activeKeyBindsPreset = 0;
	[[deprecated]] uint32_t activeSettingsPreset = 0;
	[[maybe_unused]] uint32_t _reserved_[10]{}; // NOLINT(*-avoid-c-arrays, *-magic-numbers)
};

const App::KeyBinds& App::keyBinds() const noexcept
{
	return keyBindsPresets.get(activeKeyBindsPreset).value;
}
const App::Settings& App::settings() const noexcept
{
	return settingsPresets.get(activeSettingsPreset).value;
}

static constexpr auto HeaderSize = sizeof(SaveFile::Header);
SaveFile App::decompressFile(const void* compressedData, uint32_t size)
{
	if(size < HeaderSize)
	{
		return {0, 0};
	}

	const auto* inputHeader = static_cast<const SaveFile::Header*>(compressedData);
	if(!SaveFile::isValid(*inputHeader))
	{
		return {0, 0};
	}

	std::vector<uint8_t> decompressedData(HeaderSize + inputHeader->userProp3, 0);
	new(decompressedData.data()) SaveFile::Header{*inputHeader};

	const int decompressedSize = ::sinflate(&decompressedData[HeaderSize], static_cast<int>(inputHeader->userProp3),
	    inputHeader + 1, static_cast<int>(size - HeaderSize)); // NOLINT(*-pro-bounds-pointer-arithmetic)

	if(decompressedSize != static_cast<ptrdiff_t>(inputHeader->userProp3))
	{
		return {0, 0};
	}
	return SaveFile{std::move(decompressedData)};
}
void App::storeFile(const SaveFile& save)
{
	auto deflateState = std::make_unique<::sdefl>();
	const int saveBufferSize = static_cast<int>(save.size() - HeaderSize);
	const int deflateBufferSize = ::sdefl_bound(saveBufferSize);

	auto deflateBuffer = std::vector<uint8_t>(HeaderSize + static_cast<size_t>(deflateBufferSize), 0);
	new(deflateBuffer.data()) SaveFile::Header{save.header()};

	const int deflateDataSize = ::sdeflate(deflateState.get(),
	    deflateBuffer.data() + HeaderSize,            // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	    save.data() + HeaderSize, saveBufferSize, 8); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	deflateBuffer.resize(HeaderSize + static_cast<size_t>(deflateDataSize));

#if defined(PLATFORM_WEB)
	std::vector<uint8_t>* asyncData = new std::vector<uint8_t>(std::move(deflateBuffer));
	::emscripten_idb_async_store(
	    IDB_PATH, FILE_PATH, asyncData->data(), static_cast<int>(asyncData->size()), asyncData,
	    [](void* data)
	    {
		    auto* asyncData = static_cast<std::vector<uint8_t>*>(data);
		    ::TraceLog(LOG_INFO, "FILEIO: [%s] File saved successfully", FILE_PATH);
		    delete asyncData;
	    },
	    [](void* data)
	    {
		    auto* asyncData = static_cast<std::vector<uint8_t>*>(data);
		    ::TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to save file", FILE_PATH);
		    delete asyncData;
	    });
#else
	::SaveFileData(FILE_PATH, deflateBuffer.data(), static_cast<unsigned int>(deflateBuffer.size()));
#endif
}
SaveFile App::serialize() const
{
	const size_t scoreCount = std::min<size_t>(highScores.entries.size(), std::numeric_limits<uint32_t>::max());
	const auto scoreSize = static_cast<uint32_t>(scoreCount * sizeof(HighScoreEntry));
	const uint32_t appStateSize = sizeof(HighScoreEntry::NameT) + sizeof(Settings);
	const auto keyBindPresetsSize = static_cast<uint32_t>(keyBindsPresets.size() * sizeof(Presets<KeyBinds>::Item));
	const auto settingsPresetsSize = static_cast<uint32_t>(settingsPresets.size() * sizeof(Presets<Settings>::Item));

	SaveFile save(4, scoreSize + appStateSize + keyBindPresetsSize + settingsPresetsSize);

	save.appendChunk(ChunkType::PlayerName, 0, &playerName, std::next(&playerName));
	save.appendChunk(ChunkType::HighScores, 0, highScores.entries.begin(), highScores.entries.end());
	save.appendChunk(ChunkType::KeyBindsPresets, static_cast<uint16_t>(activeKeyBindsPreset),
	    keyBindsPresets.get().begin() + static_cast<ptrdiff_t>(keyBindsPresets.fixed()), keyBindsPresets.get().end());
	save.appendChunk(ChunkType::SettingsPresets, static_cast<uint16_t>(activeSettingsPreset),
	    settingsPresets.get().begin() + static_cast<ptrdiff_t>(settingsPresets.fixed()), settingsPresets.get().end());

	save.header().userProp3 = save.size() - HeaderSize;
	return save;
}
void App::deserialize(const SaveFile& save)
{
	for(const SaveFile::Chunk::Header& chunkHeader : save)
	{
		try
		{
			switch(chunkHeader.type)
			{
			case ChunkType::PlayerName:
				playerName = *SaveFile::Chunk::DataRange<const HighScoreEntry::NameT>(chunkHeader).begin();
				break;
			case ChunkType::Settings:
			{
				const Settings& settings = *SaveFile::Chunk::DataRange<const Settings>(chunkHeader).begin();
				if(settingsPresets.find(settings) == settingsPresets.size())
				{
					settingsPresets.add({"Custom", settings});
				}
			}
			break;
			case ChunkType::HighScores:
			{
				const SaveFile::Chunk::DataRange<const HighScoreEntry> range(chunkHeader);
				highScores.entries.assign(range.begin(), range.end());
			}
			break;
			case ChunkType::KeyBinds:
			{
				const KeyBinds& keyBinds = *SaveFile::Chunk::DataRange<const KeyBinds>(chunkHeader).begin();
				if(keyBindsPresets.find(keyBinds) == keyBindsPresets.size())
				{
					keyBindsPresets.add({"Custom", keyBinds});
				}
			}
			break;
			case ChunkType::KeyBindsPresets:
			{
				const SaveFile::Chunk::DataRange<const Presets<KeyBinds>::Item> range(chunkHeader);
				keyBindsPresets.add(Range{range});
				activeKeyBindsPreset = chunkHeader.userProperty;
			}
			break;
			case ChunkType::SettingsPresets:
			{
				const SaveFile::Chunk::DataRange<const Presets<Settings>::Item> range(chunkHeader);
				settingsPresets.add(Range{range});
				activeSettingsPreset = chunkHeader.userProperty;
			}
			break;
			case ChunkType::OtherItems:
			{
				// const OtherItems& otherItems = *SaveFile::Chunk::DataRange<const OtherItems>(chunkHeader).begin();
			}
			break;
			}
		}
		catch(const std::range_error& exception)
		{
			const auto chunkTypeName = magic_enum::enum_name<ChunkType::type>(ChunkType::type{chunkHeader.type});
			::TraceLog(LOG_ERROR, "Deserialization: [%s] %s", chunkTypeName.data(), exception.what());
		}
	}
}
} // namespace raymino
