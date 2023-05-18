#include "gameplay.hpp"

#include <array>

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
Offset wallKick<WallKicks::Arika>(const Grid& field, const Tetromino& tetromino, Offset offset)
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
Offset wallKick<WallKicks::Super>(const Grid& field, const Tetromino& tetromino, Offset offset)
{
	using KickRow = std::array<XY, 4>;
	using KickTable = std::array<KickRow, 8>;
	static constexpr KickTable kicksJLSTZ{{
	    /*0->L*/ {{{+1, 0}, {+1, -1}, {0, +2}, {+1, +2}}},
	    /*0->R*/ {{{-1, 0}, {-1, -1}, {0, +2}, {-1, +2}}},
	    /*R->0*/ {{{+1, 0}, {+1, +1}, {0, -2}, {+1, -2}}},
	    /*R->2*/ {{{+1, 0}, {+1, +1}, {0, -2}, {+1, -2}}},
	    /*2->R*/ {{{-1, 0}, {-1, -1}, {0, +2}, {-1, +2}}},
	    /*2->L*/ {{{+1, 0}, {+1, -1}, {0, +2}, {+1, +2}}},
	    /*L->2*/ {{{-1, 0}, {-1, +1}, {0, -2}, {-1, -2}}},
	    /*L->0*/ {{{-1, 0}, {-1, +1}, {0, -2}, {-1, -2}}},
	}};
	static constexpr KickTable kicksI{{
	    /*0->L*/ {{{-1, 0}, {+2, 0}, {-1, -2}, {+2, -1}}},
	    /*0->R*/ {{{-2, 0}, {+1, 0}, {-2, +1}, {+1, -2}}},
	    /*R->0*/ {{{+2, 0}, {-1, 0}, {+2, -1}, {-1, +2}}},
	    /*R->2*/ {{{-1, 0}, {+2, 0}, {-1, -2}, {+2, +1}}},
	    /*2->R*/ {{{+1, 0}, {-2, 0}, {+1, +2}, {-2, -1}}},
	    /*2->L*/ {{{+2, 0}, {-1, 0}, {+2, -1}, {-1, +2}}},
	    /*L->2*/ {{{-2, 0}, {+1, 0}, {-2, +1}, {+1, -2}}},
	    /*L->0*/ {{{+1, 0}, {-2, 0}, {+1, +2}, {-2, -1}}},
	}};

	if(tetromino.type == TetrominoType::O)
	{
		return {};
	}

	const Tetromino desiredPosition{Tetromino{tetromino} += offset};
	const KickTable& kickTable = tetromino.type == TetrominoType::I ? kicksI : kicksJLSTZ;
	const size_t kickIdx = (std::abs(tetromino.rotation % 4) * 2) + static_cast<int>(offset.rotation > 0);
	const KickRow& kickRow = kickTable[kickIdx];

	for(const XY kick : kickRow)
	{
		if(field.overlapAt(desiredPosition.position + kick, desiredPosition.collision) == 0)
		{
			return {kick, offset.rotation};
		}
	}

	return {};
}
template<>
Offset wallKick<WallKicks::None>(
    [[maybe_unused]] const Grid& field, [[maybe_unused]] const Tetromino& tetromino, [[maybe_unused]] Offset offset)
{
	return {};
}

size_t eraseFullLines(Grid& grid)
{
	const auto gridWidth = grid.getSize().width;
	const auto isFullLine = [](auto begin, auto end)
	{
		return std::all_of(begin, end,
		    [](auto cell)
		    {
			    return cell != 0;
		    });
	};

	size_t erasedLines = 0;

	if(grid.getSize().area() == 0)
	{
		return erasedLines;
	}

	const auto linesEnd = grid.rend();
	auto linesReadBegin = grid.rbegin();

	while(linesReadBegin != linesEnd && !isFullLine(linesReadBegin, std::next(linesReadBegin, gridWidth)))
	{
		std::advance(linesReadBegin, gridWidth);
	}

	auto linesWriteBegin = linesReadBegin;
	if(linesReadBegin != linesEnd)
	{
		std::advance(linesReadBegin, gridWidth);
		++erasedLines;
	}

	while(linesReadBegin != linesEnd)
	{
		if(!isFullLine(linesReadBegin, std::next(linesReadBegin, gridWidth)))
		{
			std::copy(linesReadBegin, std::next(linesReadBegin, gridWidth), linesWriteBegin);
			std::advance(linesWriteBegin, gridWidth);
		}
		else
		{
			++erasedLines;
		}
		std::advance(linesReadBegin, gridWidth);
	}

	while(linesWriteBegin != linesEnd)
	{
		std::fill(linesWriteBegin, std::next(linesWriteBegin, gridWidth), uint8_t{0});
		std::advance(linesWriteBegin, gridWidth);
	}

	return erasedLines;
}

size_t countFullLines(const Grid& grid, const Tetromino& tetromino)
{
	size_t fullLines = 0;
	const auto gridSize = grid.getSize();

	for(int y = 0; y < gridSize.height; ++y)
	{
		bool isFullLine = true;
		for(int x = 0; x < gridSize.width; ++x)
		{
			if(grid.getAt({x, y}) == 0 && tetromino.collision.getAt(XY{x, y} - tetromino.position, 0) == 0)
			{
				isFullLine = false;
				break;
			}
		}
		fullLines += static_cast<size_t>(isFullLine);
	}

	return fullLines;
}

TSpinCornerCountResult tSpinCornerCount(const Grid& field, const Tetromino& tetromino)
{
	static constexpr std::array<XY, 4> checkOffsets{{{0, 0}, {2, 0}, {0, 2}, {2, 2}}};
	const Rect trueSize = findTrueSize(tetromino.collision);
	const bool horizontal = trueSize.width == 3;
	const bool topLeft = tetromino.collision.getAt({trueSize.x, trueSize.y}) == 0;
	XY trueOffset = tetromino.position + static_cast<const XY&>(trueSize);
	const auto getAt = [&](XY checkOffset)
	{
		return static_cast<int>(field.getAt(trueOffset + checkOffset) != 0);
	};

	if(horizontal && topLeft)
	{
		return {getAt(checkOffsets[0]) + getAt(checkOffsets[1]), getAt(checkOffsets[2]) + getAt(checkOffsets[3])};
	}
	if(horizontal)
	{
		trueOffset.y -= 1;
		return {getAt(checkOffsets[2]) + getAt(checkOffsets[3]), getAt(checkOffsets[0]) + getAt(checkOffsets[1])};
	}
	if(topLeft)
	{
		return {getAt(checkOffsets[0]) + getAt(checkOffsets[2]), getAt(checkOffsets[1]) + getAt(checkOffsets[3])};
	}
	trueOffset.x -= 1;
	return {getAt(checkOffsets[1]) + getAt(checkOffsets[3]), getAt(checkOffsets[0]) + getAt(checkOffsets[2])};
}

bool isImmobile(const Grid& field, const Tetromino& tetromino)
{
	static constexpr std::array<XY, 4> directions{{{0, 1}, {1, 0}, {-1, 0}, {0, -1}}};
	return std::none_of(directions.begin(), directions.end(),
	    [&](const XY dir)
	    {
		    return field.overlapAt(tetromino.position + dir, tetromino.collision) == 0;
	    });
}

template<>
ScoreEvent tSpinCheck<TSpin::Immobile>(const Grid& field, const Tetromino& tetromino, Offset offset)
{
	const Tetromino desired{Tetromino{tetromino} += offset};

	if(!isImmobile(field, desired))
	{
		return ScoreEvent::LineClear;
	}

	const size_t fullLines = countFullLines(field, tetromino);

	return fullLines > 0 ? ScoreEvent::TSpin : ScoreEvent::MiniTSpin;
}
template<>
ScoreEvent tSpinCheck<TSpin::ThreeCorner>(const Grid& field, const Tetromino& tetromino, Offset offset)
{
	if(offset.rotation == 0)
	{
		return ScoreEvent::LineClear;
	}

	const Tetromino desired{Tetromino{tetromino} += offset};
	const TSpinCornerCountResult corners = tSpinCornerCount(field, desired);

	if(corners.front == 2 && corners.back >= 1)
	{
		return ScoreEvent::TSpin;
	}
	if(corners.front == 1 && corners.back == 2)
	{
		if(offset.position.x > 1 || offset.position.y > 1)
		{
			return ScoreEvent::TSpin;
		}
		return ScoreEvent::MiniTSpin;
	}

	return ScoreEvent::LineClear;
}
template<>
ScoreEvent tSpinCheck<TSpin::Lenient>(const Grid& field, const Tetromino& tetromino, Offset offset)
{
	if(offset.rotation == 0)
	{
		return ScoreEvent::LineClear;
	}

	const Tetromino desired{Tetromino{tetromino} += offset};
	const size_t fullLines = countFullLines(field, tetromino);

	return fullLines > 0 ? ScoreEvent::TSpin : ScoreEvent::MiniTSpin;
}

struct BPS : public IScoringSystem
{
	ptrdiff_t process(ScoreEvent event, int lines, [[maybe_unused]] int level) override
	{
		switch(event)
		{
		case ScoreEvent::LineClear:
		case ScoreEvent::MiniTSpin:
		case ScoreEvent::TSpin:
		{
			lines = std::clamp(lines, 0, 4);
			static constexpr std::array<ptrdiff_t, 5> scores{0, 40, 100, 300, 1200};
			return scores[lines];
		}
		case ScoreEvent::PerfectClear:
		case ScoreEvent::SoftDrop:
		case ScoreEvent::HardDrop:
		default:
			return 0;
		}
	}
};
template<>
std::unique_ptr<IScoringSystem> makeScoringSystem<ScoringSystem::BPS>()
{
	return std::make_unique<BPS>();
}

struct Sega : public IScoringSystem
{
	ptrdiff_t process(ScoreEvent event, int lines, int level) override
	{
		lines = std::clamp(lines, 0, std::numeric_limits<int>::max());
		level = std::clamp((level + 1) / 2, 1, 5);
		switch(event)
		{
		case ScoreEvent::LineClear:
		case ScoreEvent::MiniTSpin:
		case ScoreEvent::TSpin:
		{
			lines = std::clamp(lines, 0, 4);
			static constexpr std::array<ptrdiff_t, 5> scores{0, 100, 400, 900, 2000};
			return scores[lines] * level;
		}
		case ScoreEvent::SoftDrop:
		{
			return static_cast<ptrdiff_t>(lines) * level;
		}
		case ScoreEvent::PerfectClear:
		case ScoreEvent::HardDrop:
		default:
			return 0;
		}
	}
};
template<>
std::unique_ptr<IScoringSystem> makeScoringSystem<ScoringSystem::Sega>()
{
	return std::make_unique<Sega>();
}

struct Nintendo : public IScoringSystem
{
	ptrdiff_t process(ScoreEvent event, int lines, int level) override
	{
		switch(event)
		{
		case ScoreEvent::LineClear:
		case ScoreEvent::MiniTSpin:
		case ScoreEvent::TSpin:
		{
			lines = std::clamp(lines, 0, 4);
			static constexpr std::array<ptrdiff_t, 5> scores{0, 40, 100, 300, 1200};
			return scores[lines] * level;
		}
		case ScoreEvent::PerfectClear:
		case ScoreEvent::SoftDrop:
		case ScoreEvent::HardDrop:
		default:
			return 0;
		}
	}
};
template<>
std::unique_ptr<IScoringSystem> makeScoringSystem<ScoringSystem::Nintendo>()
{
	return std::make_unique<Nintendo>();
}

struct Guideline : public IScoringSystem
{
	struct Action
	{
		enum
		{
			NoLines = 0,
			Single,
			Double,
			Triple,
			Tetris,
			MiniNoLines,
			MiniSingle,
			MiniDouble,
			SpinNoLines,
			SpinSingle,
			SpinDouble,
			SpinTriple,
			Combo,
			Soft,
			Hard,
			PerfectNoLines,
			PerfectSingle,
			PerfectDouble,
			PerfectTriple,
			PerfectTetris,
			PerfectTetrisChain,

			COUNT
		};
		ptrdiff_t points;
		bool isDifficult;
	};
	static constexpr std::array<Action, Action::COUNT> actions{{/*NoLines*/ {0, false},
	    /*Single*/ {100, false},
	    /*Double*/ {300, false},
	    /*Triple*/ {500, false},
	    /*Tetris*/ {800, true},
	    /*MiniNoLines*/ {100, false},
	    /*MiniSingle*/ {200, true},
	    /*MiniDouble*/ {400, true},
	    /*SpinNoLines*/ {400, false},
	    /*SpinSingle*/ {800, true},
	    /*SpinDouble*/ {1200, true},
	    /*SpinTriple*/ {1600, true},
	    /*Combo*/ {50, false},
	    /*Soft*/ {1, false},
	    /*Hard*/ {2, false},
	    /*PerfectNoLines*/ {0, false},
	    /*PerfectSingle*/ {800, false},
	    /*PerfectDouble*/ {1200, false},
	    /*PerfectTriple*/ {1800, false},
	    /*PerfectTetris*/ {2000, false},
	    /*PerfectTetrisChain*/ {3200, false}}};
	bool wasLastEventDifficult = false;
	int lastPerfectClearLines = 0;
	int clearCounter = 0;
	int combo = -1;
	ptrdiff_t process(ScoreEvent event, int lines, int level) override
	{
		ptrdiff_t score = 0;
		bool isThisEventDifficult = false;
		switch(event)
		{
		case ScoreEvent::LineClear:
		{
			lines = std::clamp(lines, 0, 4);
			score = actions[Action::NoLines + lines].points * level;
			isThisEventDifficult = actions[Action::NoLines + lines].isDifficult;
			break;
		}
		case ScoreEvent::MiniTSpin:
		{
			lines = std::clamp(lines, 0, 2);
			score = actions[Action::MiniNoLines + lines].points * level;
			isThisEventDifficult = actions[Action::MiniNoLines + lines].isDifficult;
			break;
		}
		case ScoreEvent::TSpin:
		{
			lines = std::clamp(lines, 0, 3);
			score = actions[Action::SpinNoLines + lines].points * level;
			isThisEventDifficult = true;
			break;
		}
		case ScoreEvent::PerfectClear:
		{
			lines = std::clamp(lines, 0, 4);
			if(lines == 4 && lastPerfectClearLines == 4 && clearCounter < 2)
			{
				return actions[Action::PerfectTetrisChain].points * level;
			}
			clearCounter = 0;
			lastPerfectClearLines = lines;
			return actions[Action::PerfectNoLines + lines].points * level;
		}
		case ScoreEvent::SoftDrop:
			return actions[Action::Soft].points * level;
		case ScoreEvent::HardDrop:
			return actions[Action::Hard].points * level;
		default:
			return 0;
		}
		if(lines > 0)
		{
			clearCounter += 1;
			combo += 1;
			score += actions[Action::Combo].points * combo * level;
		}
		else
		{
			combo = -1;
		}
		if(wasLastEventDifficult && isThisEventDifficult)
		{
			score += score / 2; // * 1.5
		}
		else
		{
			wasLastEventDifficult = isThisEventDifficult;
		}
		return score;
	}
};
template<>
std::unique_ptr<IScoringSystem> makeScoringSystem<ScoringSystem::Guideline>()
{
	return std::make_unique<Guideline>();
}
} // namespace raymino
