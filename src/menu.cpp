#include "menu.hpp"

#include "app.hpp"
#include "dependency_info.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>

#define MAGIC_ENUM_RANGE_MIN 0   // NOLINT(cppcoreguidelines-macro-usage)
#define MAGIC_ENUM_RANGE_MAX 350 // NOLINT(cppcoreguidelines-macro-usage)
#include <magic_enum.hpp>

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

template<typename TEnum, typename TContainer>
void copyEnumName(int fromKey, TContainer& intoBuffer)
{
	const auto name = magic_enum::enum_name(static_cast<TEnum>(fromKey));
	const auto last = std::copy_n(name.begin(), std::min(name.size(), intoBuffer.size() - 1), intoBuffer.begin());
	*last = '\0';
}

Menu::Menu(App& app) : // NOLINT(*-member-init) handled by readSettings
    TextBoxPlayerNameBuffer{app.playerName}, TextBoxSeedBuffer{app.seed}
{
	readSettings(app.settings);
	const App::Settings defaultSettings;
	if(defaultSettings != app.settings)
	{
		DropdownBoxPresetsActive = 0;
	}
	updateKeyBindBuffers(app.keyBinds);
}

void Menu::updateKeyBindBuffers(const App::KeyBinds& keyBinds)
{
	copyEnumName<::KeyboardKey>(keyBinds.moveRight, TextBoxMoveRightBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.moveLeft, TextBoxMoveLeftBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.rotateRight, TextBoxRotateRightBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.rotateLeft, TextBoxRotateLeftBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.softDrop, TextBoxSoftDropBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.hardDrop, TextBoxHardDropBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.hold, TextBoxHoldBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.pause, TextBoxPauseBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.restart, TextBoxRestartBuffer);
	copyEnumName<::KeyboardKey>(keyBinds.menu, TextBoxMenuBuffer);
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
	    DropdownBoxPresetsEditMode || DropdownBoxGhostPieceEditMode || DropdownBoxLevelGoalEditMode ||
	    AboutDialogShowing)
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
		app.seed = TextBoxSeedBuffer;
		app.QueueSceneSwitch(MakeScene<Scene::Game>(app));
	}
	if(GuiTextBox(TextBoxPlayerNameRect, TextBoxPlayerNameBuffer.data(),
	       static_cast<int>(TextBoxPlayerNameBuffer.size()), TextBoxPlayerNameEditMode))
	{
		TextBoxPlayerNameEditMode = !TextBoxPlayerNameEditMode;
	}
	app.playerName = TextBoxPlayerNameBuffer;
	if(GuiButton(ButtonHighscoresRect, state == State::HighScores ? GroupBoxSettingsText : ButtonHighscoresText))
	{
		state = state == State::HighScores ? State::Settings : State::HighScores;
	}
	GuiSetStyle(GuiControl::DEFAULT, GuiDefaultProperty::TEXT_SIZE, 10);

	switch(state)
	{
	case State::Settings:
		UpdateDrawSettings(app);
		break;
	case State::HighScores:
		UpdateDrawHighscores(app);
		break;
	case State::KeyBinds:
		UpdateDrawKeyBinds(app);
		break;
	}

	if(::GuiButton({GroupBoxGameRect.x + GroupBoxGameRect.width - 22, GroupBoxGameRect.y - 7, 16, 16}, "?"))
	{
		AboutDialogShowing = true;
	}
	if(AboutDialogShowing)
	{
		UpdateDrawAbout(app);
	}

	GuiUnlock();

	EndDrawing();
}

void Menu::UpdateDrawSettings(App& app)
{
	App::Settings previous;
	writeSettings(previous);

	GuiGroupBox(GroupBoxSettingsRect, GroupBoxSettingsText);
	GuiLabel(LabelA1Rect, LabelRotationSystemText);
	GuiLabel(LabelA2Rect, LabelWallKicksText);
	GuiLabel(LabelA3Rect, LabelLockDownText);
	GuiLabel(LabelA4Rect, LabelSoftDropText);
	GuiLabel(LabelA5Rect, LabelInstantDropText);
	GuiLabel(LabelB1Rect, LabelTSpinText);
	GuiLabel(LabelB2Rect, LabelShuffleTypeText);
	GuiLabel(LabelB3Rect, LabelScoringSystemText);
	GuiLabel(LabelA6Rect, LabelFieldWidthText);
	GuiLabel(LabelA7Rect, LabelFieldHeightText);
	GuiLabel(LabelA8Rect, LabelPreviewCountText);
	GuiLabel(LabelB4Rect, LabelHoldPieceText);
	GuiLabel(LabelB6Rect, LabelGhostPieceText);
	GuiLabel(LabelPresetsRect, LabelPresetsText);
	GuiLabel(LabelB5Rect, LabelLevelGoalText);
	::GuiLabel(LabelB7Rect, LabelSeedText);
	if(GuiButton(InputB8Rect, ButtonKeyBindsText))
	{
		state = State::KeyBinds;
	}
	GuiSpinner(InputA8Rect, "", SpinnerPreviewCountValue, SpinnerPreviewCountMin, SpinnerPreviewCountMax,
	    SpinnerPreviewCountEditMode);
	GuiSpinner(
	    InputA6Rect, "", SpinnerFieldWidthValue, SpinnerFieldWidthMin, SpinnerFieldWidthMax, SpinnerFieldWidthEditMode);
	GuiSpinner(InputA7Rect, "", SpinnerFieldHeightValue, SpinnerFieldHeightMin, SpinnerFieldHeightMax,
	    SpinnerFieldHeightEditMode);
	GuiTextBox(InputB7Rect, TextBoxSeedBuffer, TextBoxSeedEditMode, 17);
	GuiDropdownBox(InputB6Rect, DropdownBoxGhostPieceText, DropdownBoxGhostPieceActive, DropdownBoxGhostPieceEditMode);
	GuiDropdownBox(InputB5Rect, DropdownBoxLevelGoalText, DropdownBoxLevelGoalActive, DropdownBoxLevelGoalEditMode);
	GuiDropdownBox(InputB4Rect, DropdownBoxHoldPieceText, DropdownBoxHoldPieceActive, DropdownBoxHoldPieceEditMode);
	GuiDropdownBox(
	    InputB3Rect, DropdownBoxScoringSystemText, DropdownBoxScoringSystemActive, DropdownBoxScoringSystemEditMode);
	GuiDropdownBox(
	    InputB2Rect, DropdownBoxShuffleTypeText, DropdownBoxShuffleTypeActive, DropdownBoxShuffleTypeEditMode);
	GuiDropdownBox(InputB1Rect, DropdownBoxTSpinText, DropdownBoxTSpinActive, DropdownBoxTSpinEditMode);
	GuiDropdownBox(
	    InputA5Rect, DropdownBoxInstantDropText, DropdownBoxInstantDropActive, DropdownBoxInstantDropEditMode);
	GuiDropdownBox(InputA4Rect, DropdownBoxSoftDropText, DropdownBoxSoftDropActive, DropdownBoxSoftDropEditMode);
	GuiDropdownBox(InputA3Rect, DropdownBoxLockDownText, DropdownBoxLockDownActive, DropdownBoxLockDownEditMode);
	GuiDropdownBox(InputA2Rect, DropdownBoxWallKicksText, DropdownBoxWallKicksActive, DropdownBoxWallKicksEditMode);
	GuiDropdownBox(
	    InputA1Rect, DropdownBoxRotationSystemText, DropdownBoxRotationSystemActive, DropdownBoxRotationSystemEditMode);

	App::Settings current;
	writeSettings(current);
	if(current != previous)
	{
		writeSettings(app.settings);
		DropdownBoxPresetsActive = 0;
	}

	if(GuiDropdownBox(
	       DropdownBoxPresetsRect, DropdownBoxPresetsText, DropdownBoxPresetsActive, DropdownBoxPresetsEditMode))
	{
		if(!DropdownBoxPresetsEditMode)
		{
			switch(DropdownBoxPresetsActive)
			{
			case 1:
				readSettings(App::Settings{});
				writeSettings(app.settings);
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
	std::mt19937_64 rng(std::random_device{}());
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

void guiKeyBind(const ::Rectangle& rectLabel, const char* textLabel, const ::Rectangle& rectInput,
    Menu::KeyBufferT& keyBuffer, bool& editMode, int16_t& keyBind)
{
	::GuiLabel(rectLabel, textLabel);
	GuiTextBox(rectInput, keyBuffer, editMode);
	if(editMode)
	{
		const int key = ::GetKeyPressed();
		if(key != KEY_NULL)
		{
			copyEnumName<::KeyboardKey>(key, keyBuffer);
			editMode = false;
			keyBind = static_cast<int16_t>(key);
		}
	}
}

void Menu::UpdateDrawKeyBinds(App& app)
{
	::GuiGroupBox(GroupBoxSettingsRect, ButtonKeyBindsText);
	if(::GuiButton(InputB8Rect, GroupBoxSettingsText))
	{
		state = State::Settings;
	}
	guiKeyBind(LabelB1Rect, LabelMoveRightText, InputB1Rect, TextBoxMoveRightBuffer, TextBoxMoveRightEditMode,
	    app.keyBinds.moveRight);
	guiKeyBind(LabelA1Rect, LabelMoveLeftText, InputA1Rect, TextBoxMoveLeftBuffer, TextBoxMoveLeftEditMode,
	    app.keyBinds.moveLeft);
	guiKeyBind(LabelB2Rect, LabelRotateRightText, InputB2Rect, TextBoxRotateRightBuffer, TextBoxRotateRightEditMode,
	    app.keyBinds.rotateRight);
	guiKeyBind(LabelA2Rect, LabelRotateLeftText, InputA2Rect, TextBoxRotateLeftBuffer, TextBoxRotateLeftEditMode,
	    app.keyBinds.rotateLeft);
	guiKeyBind(LabelA3Rect, LabelSoftDropText, InputA3Rect, TextBoxSoftDropBuffer, TextBoxSoftDropEditMode,
	    app.keyBinds.softDrop);
	guiKeyBind(LabelB3Rect, LabelHardDropText, InputB3Rect, TextBoxHardDropBuffer, TextBoxHardDropEditMode,
	    app.keyBinds.hardDrop);
	guiKeyBind(LabelA4Rect, LabelHoldText, InputA4Rect, TextBoxHoldBuffer, TextBoxHoldEditMode, app.keyBinds.hold);
	guiKeyBind(LabelA5Rect, LabelPauseText, InputA5Rect, TextBoxPauseBuffer, TextBoxPauseEditMode, app.keyBinds.pause);
	guiKeyBind(
	    LabelB5Rect, LabelRestartText, InputB5Rect, TextBoxRestartBuffer, TextBoxRestartEditMode, app.keyBinds.restart);
	guiKeyBind(LabelB4Rect, LabelMenuText, InputB4Rect, TextBoxMenuBuffer, TextBoxMenuEditMode, app.keyBinds.menu);
}

void Menu::UpdateDrawAbout([[maybe_unused]] App& app)
{
	const float entryHeight = 24;
	const float totalEntriesHeight = (entryHeight * (DEPENDENCY_INFOS.size() + 2)) + 30;
	const float aboutYOffset = (static_cast<float>(App::Settings::SCREEN_HEIGHT) / 2) - (totalEntriesHeight / 2);
	const Rectangle aboutBounds{150, aboutYOffset, 300, totalEntriesHeight};
	const bool clickedOutside =
	    ::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !::CheckCollisionPointRec(::GetMousePosition(), aboutBounds);
	::GuiUnlock();
	if(::GuiWindowBox(aboutBounds, "About") || clickedOutside)
	{
		AboutDialogShowing = false;
		return;
	}

	const int prevTextColorProp = ::GuiGetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_COLOR_NORMAL);
	::GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_COLOR_NORMAL, ColorToInt(BLUE));

	if(::GuiLabelButton({aboutBounds.x + 10, aboutBounds.y + 30, 0, 20}, PROJECT_INFO.url.data()))
	{
		::OpenURL(PROJECT_INFO.url.data());
	}
	float entryYOffset = (entryHeight * 2) + 30;
	for(const LibraryInfo& dep : DEPENDENCY_INFOS)
	{
		if(::GuiLabelButton({aboutBounds.x + 10, aboutBounds.y + entryYOffset, 0, 20}, dep.url.data()))
		{
			::OpenURL(dep.url.data());
		}
		entryYOffset += entryHeight;
	}

	::GuiSetStyle(GuiControl::LABEL, GuiControlProperty::TEXT_COLOR_NORMAL, prevTextColorProp);

	::GuiLabel({aboutBounds.x + 20, aboutBounds.y + 54, 0, 20}, "build with:");

	::GuiLabel({aboutBounds.x + 250, aboutBounds.y + 30, 0, 20}, PROJECT_INFO.lic.data());
	entryYOffset = (entryHeight * 2) + 30;
	for(const LibraryInfo& dep : DEPENDENCY_INFOS)
	{
		::GuiLabel({aboutBounds.x + 235, aboutBounds.y + entryYOffset, 0, 20}, dep.lic.data());
		entryYOffset += entryHeight;
	}
}
} // namespace raymino
