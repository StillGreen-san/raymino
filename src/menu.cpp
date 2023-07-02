#include "menu.hpp"

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>

#include <array>
#include <charconv>
#include <functional>
#include <random>

namespace raymino
{
template<>
std::unique_ptr<IScene> MakeScene<Scene::Menu>(App& app)
{
	return std::make_unique<Menu>(app);
}

Menu::Menu(App& app) : TextBoxPlayerNameBuffer{app.playerName} // NOLINT(*-member-init) handled by readSettings
{
	readSettings(app.settings);
	const App::Settings defaultSettings;
	if(defaultSettings != app.settings)
	{
		DropdownBoxPresetsActive = 0;
	}
}

void Menu::readSettings(const App::Settings& settings)
{
	DropdownBoxRotationSystemActive = static_cast<int>(settings.rotationSystem);
	DropdownBoxWallKicksActive = static_cast<int>(settings.wallKicks);
	DropdownBoxLockDownActive = static_cast<int>(settings.lockDown);
	DropdownBoxSoftDropActive = static_cast<int>(settings.softDrop);
	DropdownBoxInstantDropActive = static_cast<int>(settings.instantDrop);
	DropdownBoxTSpinActive = static_cast<int>(settings.tSpin);
	DropdownBoxShuffleTypeActive = static_cast<int>(settings.shuffleType);
	DropdownBoxScoringSystemActive = static_cast<int>(settings.scoringSystem);
	DropdownBoxLevelGoalActive = static_cast<int>(settings.levelGoal);
	DropdownBoxHoldPieceActive = static_cast<int>(settings.holdPiece);
	DropdownBoxGhostPieceActive = static_cast<int>(settings.ghostPiece);
	SpinnerPreviewCountValue = settings.previewCount;
	SpinnerFieldWidthValue = settings.fieldWidth;
	SpinnerFieldHeightValue = settings.fieldHeight;
}

void Menu::writeSettings(App::Settings& settings) const
{
	settings.rotationSystem = static_cast<RotationSystem>(DropdownBoxRotationSystemActive);
	settings.wallKicks = static_cast<WallKicks>(DropdownBoxWallKicksActive);
	settings.lockDown = static_cast<LockDown>(DropdownBoxLockDownActive);
	settings.softDrop = static_cast<SoftDrop>(DropdownBoxSoftDropActive);
	settings.instantDrop = static_cast<InstantDrop>(DropdownBoxInstantDropActive);
	settings.tSpin = static_cast<TSpin>(DropdownBoxTSpinActive);
	settings.shuffleType = static_cast<ShuffleType>(DropdownBoxShuffleTypeActive);
	settings.scoringSystem = static_cast<ScoringSystem>(DropdownBoxScoringSystemActive);
	settings.levelGoal = static_cast<LevelGoal>(DropdownBoxLevelGoalActive);
	settings.holdPiece = static_cast<bool>(DropdownBoxHoldPieceActive);
	settings.ghostPiece = static_cast<bool>(DropdownBoxGhostPieceActive);
	settings.previewCount = static_cast<uint8_t>(SpinnerPreviewCountValue);
	settings.fieldWidth = static_cast<uint8_t>(SpinnerFieldWidthValue);
	settings.fieldHeight = static_cast<uint8_t>(SpinnerFieldHeightValue);
}

void Menu::UpdateDraw(App& app)
{
	BeginDrawing();

	ClearBackground(GetColor(static_cast<unsigned>(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));

	if(DropdownBoxRotationSystemEditMode || DropdownBoxWallKicksEditMode || DropdownBoxLockDownEditMode ||
	    DropdownBoxSoftDropEditMode || DropdownBoxInstantDropEditMode || DropdownBoxTSpinEditMode ||
	    DropdownBoxShuffleTypeEditMode || DropdownBoxScoringSystemEditMode || DropdownBoxHoldPieceEditMode ||
	    DropdownBoxPresetsEditMode)
	{
		GuiLock();
	}

	GuiGroupBox(GroupBoxGameRect, GroupBoxGameText);
	GuiLabel(LabelPlayerNameRect, "Player Name");
	GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, 20);
	if(GuiButton(ButtonStartGameRect, ButtonStartGameText))
	{
		writeSettings(app.settings);
		app.playerName = TextBoxPlayerNameBuffer;
		app.QueueSceneSwitch(MakeScene<Scene::Game>(app));
	}
	if(GuiTextBox(TextBoxPlayerNameRect, TextBoxPlayerNameBuffer.data(),
	       static_cast<int>(TextBoxPlayerNameBuffer.size()), TextBoxPlayerNameEditMode))
	{
		TextBoxPlayerNameEditMode = !TextBoxPlayerNameEditMode;
	}
	if(GuiButton(ButtonHighscoresRect, state == State::HighScores ? GroupBoxSettingsText : ButtonHighscoresText))
	{
		state = state == State::HighScores ? State::Settings : State::HighScores;
	}
	GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, 10);

	switch(state)
	{
	case State::Settings:
		UpdateDrawSettings();
		break;
	case State::HighScores:
		UpdateDrawHighscores(app);
		break;
	case State::KeyBinds:
		UpdateDrawKeyBinds(app);
		break;
	}

	GuiUnlock();

	EndDrawing();
}

void Menu::UpdateDrawSettings()
{
	App::Settings previous;
	writeSettings(previous);

	GuiGroupBox(GroupBoxSettingsRect, GroupBoxSettingsText);
	GuiLabel(LabelRotationSystemRect, LabelRotationSystemText);
	GuiLabel(LabelWallKicksRect, LabelWallKicksText);
	GuiLabel(LabelLockDownRect, LabelLockDownText);
	GuiLabel(LabelSoftDropRect, LabelSoftDropText);
	GuiLabel(LabelInstantDropRect, LabelInstantDropText);
	GuiLabel(LabelTSpinRect, LabelTSpinText);
	GuiLabel(LabelShuffleTypeRect, LabelShuffleTypeText);
	GuiLabel(LabelScoringSystemRect, LabelScoringSystemText);
	GuiLabel(LabelFieldWidthRect, LabelFieldWidthText);
	GuiLabel(LabelFieldHeightRect, LabelFieldHeightText);
	GuiLabel(LabelPreviewCountRect, LabelPreviewCountText);
	GuiLabel(LabelHoldPieceRect, LabelHoldPieceText);
	GuiLabel(LabelGhostPieceRect, LabelGhostPieceText);
	GuiLabel(LabelPresetsRect, LabelPresetsText);
	GuiLabel(LabelLevelGoalRect, LabelLevelGoalText);
	if(GuiSpinner(SpinnerPreviewCountRect, "", &SpinnerPreviewCountValue, SpinnerPreviewCountMin,
	       SpinnerPreviewCountMax, SpinnerPreviewCountEditMode))
	{
		SpinnerPreviewCountEditMode = !SpinnerPreviewCountEditMode;
	}
	if(GuiSpinner(SpinnerFieldWidthRect, "", &SpinnerFieldWidthValue, SpinnerFieldWidthMin, SpinnerFieldWidthMax,
	       SpinnerFieldWidthEditMode))
	{
		SpinnerFieldWidthEditMode = !SpinnerFieldWidthEditMode;
	}
	if(GuiSpinner(SpinnerFieldHeightRect, "", &SpinnerFieldHeightValue, SpinnerFieldHeightMin, SpinnerFieldHeightMax,
	       SpinnerFieldHeightEditMode))
	{
		SpinnerFieldHeightEditMode = !SpinnerFieldHeightEditMode;
	}
	if(GuiDropdownBox(DropdownBoxGhostPieceRect, DropdownBoxGhostPieceText, &DropdownBoxGhostPieceActive,
	       DropdownBoxGhostPieceEditMode))
	{
		DropdownBoxGhostPieceEditMode = !DropdownBoxGhostPieceEditMode;
	}
	if(GuiDropdownBox(DropdownBoxLevelGoalRect, DropdownBoxLevelGoalText, &DropdownBoxLevelGoalActive,
	       DropdownBoxLevelGoalEditMode))
	{
		DropdownBoxLevelGoalEditMode = !DropdownBoxLevelGoalEditMode;
	}
	if(GuiDropdownBox(DropdownBoxHoldPieceRect, DropdownBoxHoldPieceText, &DropdownBoxHoldPieceActive,
	       DropdownBoxHoldPieceEditMode))
	{
		DropdownBoxHoldPieceEditMode = !DropdownBoxHoldPieceEditMode;
	}
	if(GuiDropdownBox(DropdownBoxScoringSystemRect, DropdownBoxScoringSystemText, &DropdownBoxScoringSystemActive,
	       DropdownBoxScoringSystemEditMode))
	{
		DropdownBoxScoringSystemEditMode = !DropdownBoxScoringSystemEditMode;
	}
	if(GuiDropdownBox(DropdownBoxShuffleTypeRect, DropdownBoxShuffleTypeText, &DropdownBoxShuffleTypeActive,
	       DropdownBoxShuffleTypeEditMode))
	{
		DropdownBoxShuffleTypeEditMode = !DropdownBoxShuffleTypeEditMode;
	}
	if(GuiDropdownBox(DropdownBoxTSpinRect, DropdownBoxTSpinText, &DropdownBoxTSpinActive, DropdownBoxTSpinEditMode))
	{
		DropdownBoxTSpinEditMode = !DropdownBoxTSpinEditMode;
	}
	if(GuiDropdownBox(DropdownBoxInstantDropRect, DropdownBoxInstantDropText, &DropdownBoxInstantDropActive,
	       DropdownBoxInstantDropEditMode))
	{
		DropdownBoxInstantDropEditMode = !DropdownBoxInstantDropEditMode;
	}
	if(GuiDropdownBox(
	       DropdownBoxSoftDropRect, DropdownBoxSoftDropText, &DropdownBoxSoftDropActive, DropdownBoxSoftDropEditMode))
	{
		DropdownBoxSoftDropEditMode = !DropdownBoxSoftDropEditMode;
	}
	if(GuiDropdownBox(
	       DropdownBoxLockDownRect, DropdownBoxLockDownText, &DropdownBoxLockDownActive, DropdownBoxLockDownEditMode))
	{
		DropdownBoxLockDownEditMode = !DropdownBoxLockDownEditMode;
	}
	if(GuiDropdownBox(DropdownBoxWallKicksRect, DropdownBoxWallKicksText, &DropdownBoxWallKicksActive,
	       DropdownBoxWallKicksEditMode))
	{
		DropdownBoxWallKicksEditMode = !DropdownBoxWallKicksEditMode;
	}
	if(GuiDropdownBox(DropdownBoxRotationSystemRect, DropdownBoxRotationSystemText, &DropdownBoxRotationSystemActive,
	       DropdownBoxRotationSystemEditMode))
	{
		DropdownBoxRotationSystemEditMode = !DropdownBoxRotationSystemEditMode;
	}

	App::Settings current;
	writeSettings(current);
	if(current != previous)
	{
		DropdownBoxPresetsActive = 0;
	}

	if(GuiDropdownBox(
	       DropdownBoxPresetsRect, DropdownBoxPresetsText, &DropdownBoxPresetsActive, DropdownBoxPresetsEditMode))
	{
		DropdownBoxPresetsEditMode = !DropdownBoxPresetsEditMode;
		if(!DropdownBoxPresetsEditMode)
		{
			switch(DropdownBoxPresetsActive)
			{
			case 1:
				readSettings(App::Settings{});
			}
		}
	}
}

void drawEntry(int scoresIdx, const Rectangle& bounds, const App::HighScoreEntry& entry)
{
	std::array<char, 16> buffer{};
	std::to_chars(&buffer.front(), &buffer.back(), entry.score);
	GuiLabel({bounds.x + 15, bounds.y + (25 * scoresIdx) + 5, 65, 24}, entry.name.data());
	GuiLabel({bounds.x + 15 + 70, bounds.y + (25 * scoresIdx) + 5, 65, 24}, buffer.data());
}

void genEntries(App& app, int entryCount, const char* namePtr, const App::Settings* setPtr)
{
	std::default_random_engine rng(std::random_device{}());
	std::uniform_int_distribution<int> charDist(' ', '~');
	std::uniform_int_distribution<int> scoreDist(99, 999999);
	std::uniform_int_distribution<int> settDist6(0, 5);
	std::uniform_int_distribution<int> settDist4(0, 3);
	std::uniform_int_distribution<int> settDist3(0, 2);
	std::uniform_int_distribution<int> settDist2(0, 1);
	App::Settings settings;
	App::HighScoreEntry::NameT name{};
	for(int i = 0; i < entryCount; ++i)
	{
		if(!namePtr)
		{
			std::for_each(name.begin(), name.end(),
			    [&](auto& chr)
			    {
				    chr = static_cast<char>(charDist(rng));
			    });
		}
		if(!setPtr)
		{
			settings.rotationSystem = static_cast<RotationSystem>(settDist6(rng));
			settings.wallKicks = static_cast<WallKicks>(settDist3(rng));
			settings.lockDown = static_cast<LockDown>(settDist4(rng));
			settings.softDrop = static_cast<SoftDrop>(settDist2(rng));
			settings.instantDrop = static_cast<InstantDrop>(settDist2(rng));
			settings.tSpin = static_cast<TSpin>(settDist3(rng));
			settings.shuffleType = static_cast<ShuffleType>(settDist4(rng));
			settings.scoringSystem = static_cast<ScoringSystem>(settDist4(rng));
			settings.levelGoal = static_cast<LevelGoal>(settDist2(rng));
			settings.holdPiece = static_cast<bool>(settDist2(rng));
			settings.ghostPiece = static_cast<bool>(settDist2(rng));
			settings.previewCount = static_cast<uint8_t>(settDist6(rng));
			settings.fieldWidth = static_cast<uint8_t>(settDist6(rng) + 10);
			settings.fieldHeight = static_cast<uint8_t>(settDist6(rng) + 10);
		}
		app.highScores.add(namePtr ? namePtr : name.data(), scoreDist(rng), setPtr ? *setPtr : settings);
	}
	App::storeFile(app.serialize());
}

void drawClose(const Rectangle& bounds, const char* text, App& app, [[maybe_unused]] const char* namePtr,
    [[maybe_unused]] const App::Settings* setPtr, std::function<bool(const App::HighScoreEntry&)> selector)
{
	GuiGroupBox(bounds, text);
	if(::GuiButton({bounds.x + bounds.width - 22, bounds.y - 6, 16, 16}, "X"))
	{
		app.highScores.entries.erase(
		    std::remove_if(app.highScores.entries.begin(), app.highScores.entries.end(), selector),
		    app.highScores.entries.end());
	}
#ifndef NDEBUG
	if(::GuiButton({bounds.x + bounds.width - 45, bounds.y - 6, 16, 16}, "+"))
	{
		genEntries(app, 20, namePtr, setPtr);
	}
#endif
}

void Menu::UpdateDrawHighscores(App& app)
{
	GuiGroupBox(GroupBoxSettingsRect, ButtonHighscoresText);
	drawClose(AllScoreRect, "All Scores", app, nullptr, nullptr,
	    [&]([[maybe_unused]] const App::HighScoreEntry& entry)
	    {
		    return true;
	    });
	drawClose(MyScoreRect, "Same Name", app, app.playerName.data(), nullptr,
	    [&](const App::HighScoreEntry& entry)
	    {
		    return entry.name == app.playerName;
	    });
	drawClose(SetScoreRect, "Same Settings", app, nullptr, &app.settings,
	    [&](const App::HighScoreEntry& entry)
	    {
		    return entry.settings == app.settings;
	    });
	int allScoresIdx = 0;
	int myScoresIdx = 0;
	int setScoresIdx = 0;
	constexpr int maxIdx = 17;
	for(const App::HighScoreEntry& entry : app.highScores.entries)
	{
		if(allScoresIdx >= maxIdx && myScoresIdx >= maxIdx && setScoresIdx >= maxIdx)
		{
			break;
		}
		if(allScoresIdx < maxIdx)
		{
			drawEntry(allScoresIdx, AllScoreRect, entry);
			++allScoresIdx;
		}
		if(myScoresIdx < maxIdx && entry.name == app.playerName)
		{
			drawEntry(myScoresIdx, MyScoreRect, entry);
			++myScoresIdx;
		}
		if(setScoresIdx < maxIdx && entry.settings == app.settings)
		{
			drawEntry(setScoresIdx, SetScoreRect, entry);
			++setScoresIdx;
		}
	}
}

void Menu::UpdateDrawKeyBinds([[maybe_unused]] App& app)
{
	GuiGroupBox(GroupBoxSettingsRect, ButtonKeyBindsText);
}
} // namespace raymino
