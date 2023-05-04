#include "gameplay.hpp"

namespace raymino
{
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Super>()
{
	return {{{{0, 0}, 0}, TetrominoType::I, {{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}}},
	    {{{0, 0}, 0}, TetrominoType::J, {{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}}},
	    {{{0, 0}, 0}, TetrominoType::L, {{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}},
	    {{{0, 0}, 0}, TetrominoType::O, {{2, 2}, {1, 1, 1, 1}}},
	    {{{0, 0}, 0}, TetrominoType::S, {{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}}},
	    {{{0, 0}, 0}, TetrominoType::T, {{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}}},
	    {{{0, 0}, 0}, TetrominoType::Z, {{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}}};
}
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::Original>()
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Super>();
	find(tetrominos, TetrominoType::J)->collision.rotate(2);
	find(tetrominos, TetrominoType::L)->collision.rotate(2);
	find(tetrominos, TetrominoType::S)->collision.rotate(2);
	find(tetrominos, TetrominoType::T)->collision.rotate(2);
	find(tetrominos, TetrominoType::Z)->collision.rotate(2);
	return tetrominos;
}
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::NintendoLeft>()
{
	std::vector<Tetromino> tetrominos = makeBaseMinos<RotationSystem::Original>();
	find(tetrominos, TetrominoType::I)->collision.rotate(2);
	return tetrominos;
}
template<>
std::vector<Tetromino> makeBaseMinos<RotationSystem::NintendoRight>()
{
	return makeBaseMinos<RotationSystem::NintendoLeft>();
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
template<>
Offset basicRotation<RotationSystem::NintendoLeft>(const Tetromino& mino, int rotation)
{
	rotation %= 4;
	switch(mino.type)
	{
	case TetrominoType::I:
		return flip<1>(mino, rotation);
	case TetrominoType::S:
	case TetrominoType::Z:
		return flip<-1>(mino, rotation);
	case TetrominoType::L:
	case TetrominoType::J:
	case TetrominoType::T:
		return {{0, 0}, rotation};
	case TetrominoType::O:
		return {{0, 0}, 0};
	}
}
template<>
Offset basicRotation<RotationSystem::NintendoRight>(const Tetromino& mino, int rotation)
{
	rotation %= 4;
	switch(mino.type)
	{
	case TetrominoType::I:
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

Rect findTrueSize(const Grid& grid)
{
	Rect trueSize{{grid.getSize().width, grid.getSize().height}, {0, 0}};

	for(int y = 0; y < grid.getSize().height; ++y)
	{
		for(int x = 0; x < grid.getSize().width; ++x)
		{
			if(grid.getAt({x, y}) != 0)
			{
				trueSize.x = std::min(trueSize.x, x);
				trueSize.y = std::min(trueSize.y, y);
				trueSize.width = std::max(trueSize.width, x);
				trueSize.height = std::max(trueSize.height, y);
			}
		}
	}

	trueSize.width += 1;
	trueSize.height += 1;
	trueSize.width -= trueSize.x;
	trueSize.height -= trueSize.y;

	return trueSize;
}

XY spawnPosition(const Tetromino& tetromino, int highestUsedRow, int totalWidth)
{
	const Rect trueSize = findTrueSize(tetromino.collision);
	const int leftOffset = ((totalWidth - trueSize.width) / 2) - trueSize.x;
	const int topOffset = highestUsedRow - trueSize.y;
	return {leftOffset, topOffset};
}

template<>
Offset wallKick<RotationSystem::Arika>(const Grid& field, const Tetromino& tetromino, Offset offset)
{
	const Tetromino desiredPosition{Tetromino{tetromino} += offset};
	const XY right{1, 0};
	const XY left{-1, 0};
	const XY up{0, -1};
	const XY down{0, 1};

	switch(tetromino.type)
	{
	case TetrominoType::T:
		if(desiredPosition.rotation % 4 == 2 &&
		    field.overlapAt(desiredPosition.position + up, desiredPosition.collision) == 0)
		{
			return {{offset.position + up}, offset.rotation};
		}
	case TetrominoType::L:
	case TetrominoType::J:
		if(desiredPosition.rotation % 2 == 0)
		{
			const size_t overlapIdx = field.overlapAt(desiredPosition.position, desiredPosition.collision);
			if(overlapIdx == 2 || overlapIdx == 5 || overlapIdx == 8)
			{
				return {};
			}
		}
	case TetrominoType::Z:
	case TetrominoType::S:
		if(field.overlapAt(desiredPosition.position + right, desiredPosition.collision) == 0)
		{
			return {{offset.position + right}, offset.rotation};
		}
		if(field.overlapAt(desiredPosition.position + left, desiredPosition.collision) == 0)
		{
			return {{offset.position + left}, offset.rotation};
		}
		break;
	case TetrominoType::I:
		if(field.overlapAt(tetromino.position + up, tetromino.collision) != 0 ||
		    field.overlapAt(tetromino.position + down, tetromino.collision) != 0 ||
		    field.overlapAt(tetromino.position + left, tetromino.collision) != 0 ||
		    field.overlapAt(tetromino.position + right, tetromino.collision) != 0)
		{
			if(desiredPosition.rotation % 2 == 0)
			{
				if(field.overlapAt(desiredPosition.position + right, desiredPosition.collision) == 0)
				{
					return {{offset.position + right}, offset.rotation};
				}
				if(field.overlapAt(desiredPosition.position + right + right, desiredPosition.collision) == 0)
				{
					return {{offset.position + right + right}, offset.rotation};
				}
				if(field.overlapAt(desiredPosition.position + left, desiredPosition.collision) == 0)
				{
					return {{offset.position + left}, offset.rotation};
				}
			}
			else
			{
				if(field.overlapAt(desiredPosition.position + up, desiredPosition.collision) == 0)
				{
					return {{offset.position + up}, offset.rotation};
				}
				if(field.overlapAt(desiredPosition.position + up + up, desiredPosition.collision) == 0)
				{
					return {{offset.position + up + up}, offset.rotation};
				}
			}
		}
		break;
	case TetrominoType::O:
		break;
	}

	return {};
}
template<>
Offset wallKick<RotationSystem::Super>(const Grid& field, const Tetromino& tetromino, Offset offset)
{
	return {};
}
} // namespace raymino
