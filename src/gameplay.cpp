#include "gameplay.hpp"

namespace raymino
{
template<int TOddDir>
inline Offset flip(const Tetromino& mino, int rotation)
{
	if(rotation % 2 != 0)
	{
		return {{0, 0}, mino.rotation % 2 != 0 ? TOddDir : -TOddDir};
	}
	return {{0, 0}, 0};
}

template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Super>()
{
	return {{{{0, 0}, 0}, raymino::TetrominoType::I, {{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
	    {{{0, 0}, 0}, raymino::TetrominoType::J, {{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}}},
	    {{{0, 0}, 0}, raymino::TetrominoType::L, {{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}},
	    {{{0, 0}, 0}, raymino::TetrominoType::O, {{2, 2}, {1, 1, 1, 1}}},
	    {{{0, 0}, 0}, raymino::TetrominoType::S, {{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}}},
	    {{{0, 0}, 0}, raymino::TetrominoType::T, {{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}}},
	    {{{0, 0}, 0}, raymino::TetrominoType::Z, {{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}}};
}
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Original>()
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	tetrominos[1].collision.rotate(2);
	tetrominos[2].collision.rotate(2);
	tetrominos[4].collision.rotate(2);
	tetrominos[5].collision.rotate(2);
	tetrominos[6].collision.rotate(2);
	return tetrominos;
}
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Nintendo>()
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Original>();
	tetrominos[0].collision.rotate(2);
	return tetrominos;
}
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Arika>()
{
	return makeBaseMinos<RotationSystem::Original>();
}
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Sega>()
{
	return makeBaseMinos<RotationSystem::Original>();
}

template<>
Offset basicRotation<RotationSystem::Super>(const Tetromino& mino, int rotation)
{
	rotation = mino.type == TetrominoType::O ? 0 : rotation;
	return {{0, 0}, rotation};
}
template<>
Offset basicRotation<RotationSystem::Sega>(const Tetromino& mino, int rotation)
{
	rotation %= 4;
	if(rotation == 0)
	{
		return {{0, 0}, 0};
	}
	switch(mino.type)
	{
	case TetrominoType::I:
	case TetrominoType::S:
		return flip<-1>(mino, rotation);
	case TetrominoType::Z:
		return flip<1>(mino, rotation);
	case TetrominoType::L:
	case TetrominoType::J:
	case TetrominoType::T:
	{
		if((mino.rotation + rotation) % 4 == 2)
		{
			return {{0, 1}, rotation};
		}
		if(mino.rotation % 4 == 2)
		{
			return {{0, -1}, rotation};
		}
		return {{0, 0}, rotation};
	}
	case TetrominoType::O:
		return {{0, 0}, 0};
	}
}
template<>
Offset basicRotation<RotationSystem::Arika>(const Tetromino& mino, int rotation)
{
	return basicRotation<RotationSystem::Sega>(mino, rotation);
}
template<>
Offset basicRotation<RotationSystem::Original>(const Tetromino& mino, int rotation)
{
	rotation %= 4;
	switch(mino.type)
	{
	case TetrominoType::I:
		return flip<-1>(mino, rotation);
	case TetrominoType::S:
	case TetrominoType::Z:
		return flip<1>(mino, rotation);
	case TetrominoType::L:
	case TetrominoType::J:
	case TetrominoType::T:
		return {{0, 0}, rotation};
	case TetrominoType::O:
		return {{0, 0}, 0};
	}
}
} // namespace raymino
