#pragma once

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

#include <vector>

struct XY
{
	unsigned x;
	unsigned y;
};
struct Size
{
	unsigned width;
	unsigned height;
};
struct Rect : XY, Size
{
};

class Grid
{
public:
	Grid() = delete;
	Grid(Size size, uint8_t fill);
	Grid(Size size, std::vector<uint8_t> grid);
	Grid subGrid(Rect rect) const;
	void rotate(int steps);
	void setAt(XY topLeft, const Grid& other);
	using TOverlapFunc = bool(uint8_t lhs, uint8_t rhs);
	bool overlapAt(XY topLeft, const Grid& other, TOverlapFunc check) const;
	bool isSqaure() const;
	Size getSize() const;
	std::vector<uint8_t>::iterator begin();
	std::vector<uint8_t>::iterator end();
private:
	std::vector<uint8_t> cells;
	Size size;
};

struct Game : raymino::IScene
{
	enum class State
	{
		Drop,
		Set,
		Over,
	};
	struct ActiveMino
	{
		ActiveMino(const Grid& mino);
		Grid color;
		Grid collision;
		XY position;
		static bool doesOverlap(uint8_t lhs, uint8_t rhs);
	};
	Game()
	{
	}
	void dropMino();
	void setMino();
	void update(raymino::App& app)
	{
	}
	void draw()
	{
		::BeginDrawing();

		::ClearBackground(RColor::LightGray());

		::EndDrawing();
	}
	void UpdateDraw(raymino::App& app) override
	{
		update(app);
		draw();
	}
	std::vector<Grid> shuffledBaseMinos();
	ActiveMino takeNextMino();
	Grid playArea{{10, 20 + 4}, 0};
	std::vector<Grid> baseMinos{{{4, 4}, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}},
	    {{3, 3}, {1, 0, 0, 1, 1, 1, 0, 0, 0}}, {{3, 3}, {0, 0, 1, 1, 1, 1, 0, 0, 0}}, {{2, 2}, {1, 1, 1, 1}},
	    {{3, 3}, {0, 1, 1, 1, 1, 0, 0, 0, 0}}, {{3, 3}, {0, 1, 0, 1, 1, 1, 0, 0, 0}},
	    {{3, 3}, {1, 1, 0, 0, 1, 1, 0, 0, 0}}};
	std::vector<Grid> nextMinos{shuffledBaseMinos()};
	ActiveMino activeMino{takeNextMino()};
	State state{State::Drop};
};

template<>
std::unique_ptr<raymino::IScene> raymino::MakeScene<raymino::Scene::Game>()
{
	return std::make_unique<Game>();
}
