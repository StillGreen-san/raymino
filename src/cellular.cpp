#pragma once

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>
#include <rres-raylib.h>

struct Cellular : raymino::IScene
{
	RRectangle GetButtonBounds()
	{
		return {(::GetRenderWidth() / 2.0f) - (buttonWidth / 2.0f),
		    (::GetRenderHeight() / 2.0f) - (buttonHeight / 2.0f), buttonWidth, buttonHeight};
	}
	void UpdateDraw([[maybe_unused]] raymino::App& app) override
	{
		if(IsWindowResized())
		{
			::TraceLog(LOG_INFO, "RESIZED");
		}

		::BeginDrawing();

		texture.Draw(0, 0);
		::DrawFPS(10, 10);

		if(::GuiButton(GetButtonBounds(), "Regenerate"))
		{
			const RImage cells(::GenImageCellular(::GetRenderWidth(), ::GetRenderHeight(), 32));
			texture = RTexture2D(cells);
			const unsigned crc = ::rresComputeCRC32((unsigned char*)cells.GetData(), cells.GetPixelDataSize());
			::TraceLog(LOG_INFO, "crc: %x", crc);
		}

		::EndDrawing();
	}
	RTexture2D texture{RImage(::GenImageCellular(::GetRenderWidth(), ::GetRenderHeight(), 32))};
	static constexpr float buttonWidth{80.0f};
	static constexpr float buttonHeight{30.0f};
};

template<>
std::unique_ptr<raymino::IScene> raymino::MakeScene<raymino::Scene::Cellular>()
{
	return std::make_unique<Cellular>();
}
