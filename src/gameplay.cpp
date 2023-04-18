#include "gameplay.hpp"

namespace raymino
{
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Super>()
{
	return {{raymino::TetrominoType::I, 0, {0, 0}, {{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
	    {raymino::TetrominoType::J, 0, {0, 0}, {{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}}},
	    {raymino::TetrominoType::L, 0, {0, 0}, {{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}},
	    {raymino::TetrominoType::O, 0, {0, 0}, {{2, 2}, {1, 1, 1, 1}}},
	    {raymino::TetrominoType::S, 0, {0, 0}, {{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}}},
	    {raymino::TetrominoType::T, 0, {0, 0}, {{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}}},
	    {raymino::TetrominoType::Z, 0, {0, 0}, {{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}}};
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
} // namespace raymino
