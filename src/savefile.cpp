#include "savefile.hpp"

namespace raymino
{
const std::vector<uint8_t>& SaveFile::getBuffer() const
{
	return dataBuffer;
}
const uint8_t* SaveFile::data() const
{
	return dataBuffer.data();
}
uint32_t SaveFile::size() const
{
	return dataBuffer.size();
}
} // namespace raymino
