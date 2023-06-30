#include "app.hpp"

#include <endian/network.hpp>
#include <endian/stream_reader.hpp>
#include <endian/stream_writer.hpp>
#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

raymino::App::App() : playerName{}
{
	::InitWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, "raymino");
	::SetWindowState(FLAG_VSYNC_HINT);
	::SetExitKey(KEY_NULL);
	HighScoreEntry::copyInto("Mino", playerName);
	currentScene = MakeScene<Scene::Loading>(*this);
}

void UpdateDraw(void* app)
{
	raymino::App& self = *static_cast<raymino::App*>(app);
	self.UpdateDraw();
}

void raymino::App::UpdateDraw()
{
	if(nextScene)
	{
		currentScene = std::move(nextScene);
	}
	currentScene->UpdateDraw(*this);
}

void raymino::App::Run()
{
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(::UpdateDraw, this, 0, 1);
#else
	while(!::WindowShouldClose())
	{
		::UpdateDraw(this);
	}
#endif
}

void raymino::App::QueueSceneSwitch(std::unique_ptr<IScene> newScene)
{
	nextScene = std::move(newScene);
}

bool raymino::App::addHighScore(int64_t score)
{
	const bool isHighScore = highScores.add(playerName.data(), score, settings);
	if(highScores.entries.size() > MAX_SCORES)
	{
		highScores.entries.erase(next(begin(highScores.entries), MAX_SCORES), end(highScores.entries));
	}
	std::vector<unsigned char> fileData = serialize();
#if defined(PLATFORM_WEB)
	std::vector<unsigned char>* asyncData = new std::vector<unsigned char>(std::move(fileData));
	emscripten_idb_async_store(
	    IDB_PATH, FILE_PATH, asyncData->data(), asyncData->size(), asyncData,
	    [](void* data)
	    {
		    auto* asyncData = static_cast<std::vector<unsigned char>*>(data);
		    ::TraceLog(LOG_INFO, "FILEIO: [%s] File saved successfully", FILE_PATH);
		    delete asyncData;
	    },
	    [](void* data)
	    {
		    auto* asyncData = static_cast<std::vector<unsigned char>*>(data);
		    ::TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to save file", FILE_PATH);
		    delete asyncData;
	    });
#else
	::SaveFileData(FILE_PATH, fileData.data(), fileData.size());
#endif
	return isHighScore;
}

std::vector<unsigned char> raymino::App::serialize()
{
	std::vector<unsigned char> data(40 + (highScores.entries.size() * 32));
	endian::stream_writer<endian::network> endianWriter(data.data(), data.size());

	endianWriter.write(FILE_VERSION);
	endianWriter.write(reinterpret_cast<const uint8_t*>(&playerName), 8);

	endianWriter.write(reinterpret_cast<const uint8_t*>(&settings), 14);
	endianWriter.write(reinterpret_cast<const uint8_t*>("\0\0"), 2);

	endianWriter.write(highScores.entries.size());
	for(const HighScoreEntry& entry : highScores.entries)
	{
		endianWriter.write(reinterpret_cast<const uint8_t*>(&entry.name), 8);
		endianWriter.write(entry.score);
		endianWriter.write(reinterpret_cast<const uint8_t*>(&entry.settings), 14);
		endianWriter.write(reinterpret_cast<const uint8_t*>("\0\0"), 2);
	}

	return data;
}
void raymino::App::deserialize(unsigned char* data, unsigned int bytes)
{
	endian::stream_reader<endian::network> endianReader(data, bytes);

	const auto fileVersion = endianReader.read<size_t>();
	endianReader.read(reinterpret_cast<uint8_t*>(&playerName), 8);

	endianReader.read(reinterpret_cast<uint8_t*>(&settings), 14);
	endianReader.read<uint16_t>();

	highScores.entries.clear();
	highScores.entries.resize(endianReader.read<size_t>(), HighScoreEntry(playerName.data(), 0, settings));
	for(HighScoreEntry& entry : highScores.entries)
	{
		endianReader.read(reinterpret_cast<uint8_t*>(&entry.name), 8);
		endianReader.read(entry.score);
		endianReader.read(reinterpret_cast<uint8_t*>(&entry.settings), 14);
		endianReader.read<uint16_t>();
	}
}

const raymino::App::SaveFile::Header& raymino::App::SaveFile::header() const
{
	return *reinterpret_cast<const Header*>( // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	    dataBuffer.data() + sizeof(Header)); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}
const raymino::App::HighScoreEntry* raymino::App::SaveFile::begin() const
{
	return header().scoreCount > 0
	           ? reinterpret_cast<const HighScoreEntry*>( // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	                 dataBuffer.data() + sizeof(Header))  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	           : nullptr;
}
const raymino::App::HighScoreEntry* raymino::App::SaveFile::end() const
{
	return header().scoreCount > 0
	           ? reinterpret_cast<const HighScoreEntry*>(  // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	                 dataBuffer.data() + sizeof(Header)) + // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	                 header().scoreCount                   // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	           : nullptr;
}
