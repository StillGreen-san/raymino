#include "app.hpp"

#include <external/sdefl.h>
#include <external/sinfl.h>
#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace raymino
{
App::App() : playerName{}
{
	::InitWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, "raymino");
	::SetWindowState(FLAG_VSYNC_HINT);
	::SetExitKey(KEY_NULL);
	HighScoreEntry::copyInto("Mino", playerName);
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
}

void App::QueueSceneSwitch(std::unique_ptr<IScene> newScene)
{
	nextScene = std::move(newScene);
}

bool App::addHighScore(int64_t score)
{
	const bool isHighScore = highScores.add(playerName.data(), score, settings);
	if(highScores.entries.size() > MAX_SCORES)
	{
		highScores.entries.erase(next(begin(highScores.entries), MAX_SCORES), end(highScores.entries));
	}
	App::storeFile(serialize());
	return isHighScore;
}

const App::SaveFile::Header& App::SaveFile::header() const
{
	return *reinterpret_cast<const Header*>(dataBuffer.data()); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}
App::HighScoreEntry* App::SaveFile::begin()
{
	return header().scoreCount > 0
	           ? reinterpret_cast<HighScoreEntry*>(      // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	                 dataBuffer.data() + sizeof(Header)) // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	           : nullptr;
}
const App::HighScoreEntry* App::SaveFile::begin() const
{
	return header().scoreCount > 0
	           ? reinterpret_cast<const HighScoreEntry*>( // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	                 dataBuffer.data() + sizeof(Header))  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	           : nullptr;
}
const App::HighScoreEntry* App::SaveFile::end() const
{
	return header().scoreCount > 0
	           ? reinterpret_cast<const HighScoreEntry*>(  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	                 dataBuffer.data() + sizeof(Header)) + // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	                 header().scoreCount                   // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	           : nullptr;
}

bool App::SaveFile::Header::isValid() const
{
	return magic == App::SaveFile::magic && fileVersion == App::FILE_VERSION;
}

App::SaveFile App::makeSaveFile(const void* data, int size)
{
	SaveFile save;
	if(size < static_cast<ptrdiff_t>(sizeof(SaveFile::Header)))
	{
		save.dataBuffer.resize(sizeof(SaveFile::Header));
		return save;
	}
	const auto* inputHeader = static_cast<const SaveFile::Header*>(data);
	if(!inputHeader->isValid())
	{
		save.dataBuffer.resize(sizeof(SaveFile::Header));
		return save;
	}
	const size_t scoreBufferSize = inputHeader->scoreCount * sizeof(HighScoreEntry);
	save.dataBuffer.resize(sizeof(SaveFile::Header) + scoreBufferSize);
	std::memcpy(save.dataBuffer.data(), data, sizeof(SaveFile::Header));
	sinflate(save.begin(), static_cast<int>(scoreBufferSize),
	    inputHeader + 1, // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	    size - static_cast<int>(sizeof(SaveFile::Header)));
	return save;
}

void App::storeFile(const SaveFile& save)
{
	if(!save.header().isValid())
	{
		return;
	}
	auto deflateState = std::make_unique<sdefl>();
	const int scoreBufferSize = static_cast<int>(save.dataBuffer.size() - sizeof(SaveFile::Header));
	const int deflateBufferSize = sdefl_bound(scoreBufferSize);
	auto deflateBuffer = std::vector<uint8_t>(sizeof(SaveFile::Header) + static_cast<size_t>(deflateBufferSize), 0);
	std::memcpy(deflateBuffer.data(), save.dataBuffer.data(), sizeof(SaveFile::Header));
	const int deflateDataSize = sdeflate(deflateState.get(),
	    deflateBuffer.data() + sizeof(SaveFile::Header), // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	    save.begin(), scoreBufferSize, 8);
	deflateBuffer.resize(sizeof(SaveFile::Header) + static_cast<size_t>(deflateDataSize));
#if defined(PLATFORM_WEB)
	std::vector<uint8_t>* asyncData = new std::vector<uint8_t>(std::move(deflateBuffer));
	emscripten_idb_async_store(
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
	::SaveFileData(FILE_PATH, deflateBuffer.data(), deflateBuffer.size());
#endif
}

App::SaveFile App::serialize() const
{
	SaveFile save;

	save.dataBuffer.resize(sizeof(SaveFile::Header) + (highScores.entries.size() * sizeof(HighScoreEntry)));

	const size_t scoreCount = std::min<size_t>(highScores.entries.size(), std::numeric_limits<uint16_t>::max());
	new(save.dataBuffer.data())
	    SaveFile::Header{SaveFile::magic, FILE_VERSION, static_cast<uint16_t>(scoreCount), playerName, settings};

	HighScoreEntry* entryIt = save.begin();
	for(size_t scoreIdx = 0; scoreIdx < scoreCount;
	    ++scoreIdx, ++entryIt) // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	{
		new(entryIt) HighScoreEntry{highScores.entries[scoreIdx]};
	}

	return save;
}

void App::deserialize(const App::SaveFile& save)
{
	if(!save.header().isValid())
	{
		return;
	}
	playerName = save.header().playerName;
	settings = save.header().settings;
	highScores.entries.assign(save.begin(), save.end());
}

static constexpr auto HeaderSize = sizeof(raymino::SaveFile::Header);
raymino::SaveFile App::decompressFile(const void* compressedData, uint32_t size)
{
	if(size < HeaderSize)
	{
		return {0, 0};
	}

	const auto* inputHeader = static_cast<const raymino::SaveFile::Header*>(compressedData);
	std::vector<uint8_t> decompressedData(HeaderSize + inputHeader->userProp3, 0);
	new(decompressedData.data()) raymino::SaveFile::Header{*inputHeader};

	const int decompressedSize = sinflate(&decompressedData[HeaderSize], static_cast<int>(inputHeader->userProp3),
	    inputHeader + 1, static_cast<int>(size - HeaderSize)); // NOLINT(*-pro-bounds-pointer-arithmetic)

	if(decompressedSize != static_cast<ptrdiff_t>(inputHeader->userProp3))
	{
		return {0, 0};
	}
	return {std::move(decompressedData)};
}
void App::storeFile(const raymino::SaveFile& save)
{
	auto deflateState = std::make_unique<sdefl>();
	const int saveBufferSize = static_cast<int>(save.size() - HeaderSize);
	const int deflateBufferSize = sdefl_bound(saveBufferSize);

	auto deflateBuffer = std::vector<uint8_t>(HeaderSize + static_cast<size_t>(deflateBufferSize), 0);
	new(deflateBuffer.data()) raymino::SaveFile::Header{save.header()};

	const int deflateDataSize = sdeflate(deflateState.get(),
	    deflateBuffer.data() + HeaderSize,            // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	    save.data() + HeaderSize, saveBufferSize, 8); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	deflateBuffer.resize(HeaderSize + static_cast<size_t>(deflateDataSize));

#if defined(PLATFORM_WEB)
	std::vector<uint8_t>* asyncData = new std::vector<uint8_t>(std::move(deflateBuffer));
	emscripten_idb_async_store(
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
raymino::SaveFile App::serialize([[maybe_unused]] int _temporary_arg_) const
{
	const size_t scoreCount = std::min<size_t>(highScores.entries.size(), std::numeric_limits<uint32_t>::max());
	const auto scoreSize = static_cast<uint32_t>(scoreCount * sizeof(HighScoreEntry));
	const uint32_t appStateSize = sizeof(HighScoreEntry::NameT) + sizeof(Settings);

	raymino::SaveFile save(3, scoreSize + appStateSize);

	save.appendChunk(ChunkType::PlayerName, 0, &playerName, std::next(&playerName));
	save.appendChunk(ChunkType::Settings, 0, &settings, std::next(&settings));
	save.appendChunk(ChunkType::HighScores, 0, highScores.entries.begin(), highScores.entries.end());

	save.header().userProp3 = save.size() - HeaderSize;
	return save;
}
void App::deserialize(const raymino::SaveFile& save)
{
	for(const raymino::SaveFile::Chunk::Header& chunkHeader : save)
	{
		switch(chunkHeader.type)
		{
		case ChunkType::PlayerName:
			playerName = *raymino::SaveFile::Chunk::DataRange<const HighScoreEntry::NameT>(chunkHeader).begin();
			break;
		case ChunkType::Settings:
			settings = *raymino::SaveFile::Chunk::DataRange<const Settings>(chunkHeader).begin();
			break;
		case ChunkType::HighScores:
		{
			const raymino::SaveFile::Chunk::DataRange<const HighScoreEntry> range(chunkHeader);
			highScores.entries.assign(range.begin(), range.end());
		}
		break;
		}
	}
}
} // namespace raymino
