#include "savefile.hpp"

namespace raymino
{
const SaveFile::Header& SaveFile::header() const noexcept
{
	return reinterpret_cast<const Header&>(dataBuffer.front()); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}
SaveFile::Header& SaveFile::header() noexcept
{
	return reinterpret_cast<Header&>(dataBuffer.front()); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}
SaveFile::Chunk::ConstIterator SaveFile::begin() const noexcept
{
	return Chunk::ConstIterator{
	    *reinterpret_cast<const Chunk::Header*>( // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	        &header() + 1)};                     // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}
SaveFile::Chunk::ConstIterator SaveFile::end() const noexcept
{
	return Chunk::ConstIterator{
	    *reinterpret_cast<const Chunk::Header*>(     // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	        dataBuffer.data() + dataBuffer.size())}; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}
SaveFile::Chunk::Iterator SaveFile::begin() noexcept
{
	return Chunk::Iterator{*reinterpret_cast<Chunk::Header*>( // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	    &header() + 1)};                                      // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}
SaveFile::Chunk::Iterator SaveFile::end() noexcept
{
	return Chunk::Iterator{*reinterpret_cast<Chunk::Header*>( // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
	    dataBuffer.data() + dataBuffer.size())};              // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}

SaveFile::Chunk::Header& SaveFile::appendChunk(uint16_t type, uint16_t flags, uint32_t bytes)
{
	auto headerPos = dataBuffer.insert(dataBuffer.end(), sizeof(Chunk::Header) + bytes, 0);
	Chunk::Header& header = *new(&*headerPos) Chunk::Header{type, flags, bytes};
	return header;
}

const std::vector<uint8_t>& SaveFile::getBuffer() const noexcept
{
	return dataBuffer;
}
const uint8_t* SaveFile::data() const noexcept
{
	return dataBuffer.data();
}
uint32_t SaveFile::size() const noexcept
{
	return static_cast<uint32_t>(dataBuffer.size());
}

void SaveFile::reset(uint32_t chunks, uint32_t totalBytes)
{
	dataBuffer.clear();
	dataBuffer.reserve(sizeof(Header) + (sizeof(Chunk::Header) * chunks) + totalBytes);
	dataBuffer.resize(sizeof(Header));
	new(dataBuffer.data()) Header{magic, 2, 0, 0, 0, 0};
}
SaveFile::SaveFile(uint32_t chunks, uint32_t totalBytes)
{
	reset(chunks, totalBytes);
}
SaveFile::SaveFile(std::vector<uint8_t> data) : dataBuffer{std::move(data)}
{
	if(dataBuffer.size() < sizeof(Header) || !SaveFile::isValid(header()))
	{
		reset(0, 0);
	}
}
bool SaveFile::isValid(SaveFile::Header header) noexcept
{
	return header.magic == SaveFile::magic && header.formatVersion == 2 && header._reserved_ == 0;
}
} // namespace raymino
