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
void copyEnumName(int fromKey, TContainer& intoBuffer) noexcept
{
	const auto name = magic_enum::enum_name(static_cast<TEnum>(fromKey));
	const auto last = std::copy_n(name.begin(), std::min(name.size(), intoBuffer.size() - 1), intoBuffer.begin());
	*last = '\0';
}

template<typename TEnum>
TextList enumToTextList()
{
	TextList list;
	const auto enumNames = magic_enum::enum_names<TEnum>();
	for(const auto name : enumNames)
	{
		const std::string split = splitCamel(name);
		list.add(split);
	}
	return list;
}

Menu::Menu(App& app) : // NOLINT(*-member-init) handled by readSettings
    DropdownBoxRotationSystemTextList{enumToTextList<RotationSystem>()},
    DropdownBoxWallKicksTextList{enumToTextList<WallKicks>()}, DropdownBoxLockDownTextList{enumToTextList<LockDown>()},
    DropdownBoxSoftDropTextList{enumToTextList<SoftDrop>()},
    DropdownBoxInstantDropTextList{enumToTextList<InstantDrop>()}, DropdownBoxTSpinTextList{enumToTextList<TSpin>()},
    DropdownBoxShuffleTypeTextList{enumToTextList<ShuffleType>()},
    DropdownBoxScoringSystemTextList{enumToTextList<ScoringSystem>()},
    DropdownBoxLevelGoalTextList{enumToTextList<LevelGoal>()},
    keyBindsPresets{app.keyBindsPresets, app.activeKeyBindsPreset},
    settingsPresets{app.settingsPresets, app.activeSettingsPreset}, TextBoxPlayerNameBuffer{app.playerName},
    TextBoxSeedBuffer{app.seed}
{
	const App::Settings& settings = settingsPresets.getValue();
	const App::KeyBinds& keyBinds = keyBindsPresets.getValue();
	readSettings(settings);
	updateKeyBindBuffers(keyBinds);
	keyBindsPresets.getValue().moveRight = keyBinds.moveRight;
	keyBindsPresets.getValue().moveLeft = keyBinds.moveLeft;
	keyBindsPresets.getValue().rotateRight = keyBinds.rotateRight;
	keyBindsPresets.getValue().rotateLeft = keyBinds.rotateLeft;
	keyBindsPresets.getValue().softDrop = keyBinds.softDrop;
	keyBindsPresets.getValue().hardDrop = keyBinds.hardDrop;
	keyBindsPresets.getValue().hold = keyBinds.hold;
	keyBindsPresets.getValue().pause = keyBinds.pause;
	keyBindsPresets.getValue().restart = keyBinds.restart;
	keyBindsPresets.getValue().menu = keyBinds.menu;
}

void Menu::PreDestruct(raymino::App& app)
{
	app.playerName = TextBoxPlayerNameBuffer;
	app.seed = TextBoxSeedBuffer;
	app.keyBindsPresets = keyBindsPresets.getPresets();
	app.activeKeyBindsPreset = static_cast<uint32_t>(keyBindsPresets.active());
	app.settingsPresets = settingsPresets.getPresets();
	app.activeSettingsPreset = static_cast<uint32_t>(settingsPresets.active());
}

void Menu::updateKeyBindBuffers(const App::KeyBinds& keyBinds) noexcept
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

void Menu::readSettings(const App::Settings& settings) noexcept
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

void Menu::writeSettings(App::Settings& settings) const noexcept
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
	::BeginDrawing();

	::ClearBackground(::GetColor(static_cast<unsigned>(::GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));

	if(DropdownBoxRotationSystemEditMode || DropdownBoxWallKicksEditMode || DropdownBoxLockDownEditMode ||
	    DropdownBoxSoftDropEditMode || DropdownBoxInstantDropEditMode || DropdownBoxTSpinEditMode ||
	    DropdownBoxShuffleTypeEditMode || DropdownBoxScoringSystemEditMode || DropdownBoxHoldPieceEditMode ||
	    DropdownBoxGhostPieceEditMode || DropdownBoxLevelGoalEditMode || keyBindsPresets.inEditMode() ||
	    settingsPresets.inEditMode() || AboutDialogShowing)
	{
		::GuiLock();
	}

	::GuiGroupBox(GroupBoxGameRect, GroupBoxGameText);
	::GuiLabel(LabelPlayerNameRect, "Player Name");
	::GuiSetStyle(::GuiControl::DEFAULT, ::GuiDefaultProperty::TEXT_SIZE, HeadingFontSize);
	if(::GuiButton(ButtonStartGameRect, ButtonStartGameText))
	{
		app.QueueSceneSwitch(Scene::Game);
	}
	if(GuiTextBox(TextBoxPlayerNameRect, TextBoxPlayerNameBuffer, TextBoxPlayerNameEditMode))
	{
		TextBoxPlayerNameEditMode = !TextBoxPlayerNameEditMode;
	}
	if(::GuiButton(ButtonHighscoresRect, state == State::HighScores ? GroupBoxSettingsText : ButtonHighscoresText))
	{
		state = state == State::HighScores ? State::Settings : State::HighScores;
	}
	::GuiSetStyle(::GuiControl::DEFAULT, ::GuiDefaultProperty::TEXT_SIZE, TextFontSize);

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

	::GuiUnlock();

	::EndDrawing();
}

void Menu::UpdateDrawSettings([[maybe_unused]] App& app)
{
	::GuiGroupBox(GroupBoxSettingsRect, GroupBoxSettingsText);
	::GuiLabel(SettingsRects[0][0][0], LabelRotationSystemText);
	::GuiLabel(SettingsRects[0][1][0], LabelWallKicksText);
	::GuiLabel(SettingsRects[0][2][0], LabelLockDownText);
	::GuiLabel(SettingsRects[0][3][0], LabelSoftDropText);
	::GuiLabel(SettingsRects[0][4][0], LabelInstantDropText);
	::GuiLabel(SettingsRects[1][0][0], LabelTSpinText);
	::GuiLabel(SettingsRects[1][1][0], LabelShuffleTypeText);
	::GuiLabel(SettingsRects[1][2][0], LabelScoringSystemText);
	::GuiLabel(SettingsRects[0][5][0], LabelFieldWidthText);
	::GuiLabel(SettingsRects[0][6][0], LabelFieldHeightText);
	::GuiLabel(SettingsRects[0][7][0], LabelPreviewCountText);
	::GuiLabel(SettingsRects[1][3][0], LabelHoldPieceText);
	::GuiLabel(SettingsRects[1][5][0], LabelGhostPieceText);
	::GuiLabel(LabelPresetsRect, LabelPresetsText);
	::GuiLabel(SettingsRects[1][4][0], LabelLevelGoalText);
	::GuiLabel(SettingsRects[1][6][0], LabelSeedText);
	if(GuiButton(SettingsRects[1][7][1], ButtonKeyBindsText))
	{
		state = State::KeyBinds;
	}
	GuiSpinner(SettingsRects[0][7][1], "", SpinnerPreviewCountValue, SpinnerPreviewCountMin, SpinnerPreviewCountMax,
	    SpinnerPreviewCountEditMode);
	GuiSpinner(SettingsRects[0][5][1], "", SpinnerFieldWidthValue, SpinnerFieldWidthMin, SpinnerFieldWidthMax,
	    SpinnerFieldWidthEditMode);
	GuiSpinner(SettingsRects[0][6][1], "", SpinnerFieldHeightValue, SpinnerFieldHeightMin, SpinnerFieldHeightMax,
	    SpinnerFieldHeightEditMode);
	GuiTextBox(SettingsRects[1][6][1], TextBoxSeedBuffer, TextBoxSeedEditMode, 17);
	GuiDropdownBox(
	    SettingsRects[1][5][1], DropdownBoxGhostPieceText, DropdownBoxGhostPieceActive, DropdownBoxGhostPieceEditMode);
	GuiDropdownBox(
	    SettingsRects[1][4][1], DropdownBoxLevelGoalTextList, DropdownBoxLevelGoalActive, DropdownBoxLevelGoalEditMode);
	GuiDropdownBox(
	    SettingsRects[1][3][1], DropdownBoxHoldPieceText, DropdownBoxHoldPieceActive, DropdownBoxHoldPieceEditMode);
	GuiDropdownBox(SettingsRects[1][2][1], DropdownBoxScoringSystemTextList, DropdownBoxScoringSystemActive,
	    DropdownBoxScoringSystemEditMode);
	GuiDropdownBox(SettingsRects[1][1][1], DropdownBoxShuffleTypeTextList, DropdownBoxShuffleTypeActive,
	    DropdownBoxShuffleTypeEditMode);
	GuiDropdownBox(SettingsRects[1][0][1], DropdownBoxTSpinTextList, DropdownBoxTSpinActive, DropdownBoxTSpinEditMode);
	GuiDropdownBox(SettingsRects[0][4][1], DropdownBoxInstantDropTextList, DropdownBoxInstantDropActive,
	    DropdownBoxInstantDropEditMode);
	GuiDropdownBox(
	    SettingsRects[0][3][1], DropdownBoxSoftDropTextList, DropdownBoxSoftDropActive, DropdownBoxSoftDropEditMode);
	GuiDropdownBox(
	    SettingsRects[0][2][1], DropdownBoxLockDownTextList, DropdownBoxLockDownActive, DropdownBoxLockDownEditMode);
	GuiDropdownBox(
	    SettingsRects[0][1][1], DropdownBoxWallKicksTextList, DropdownBoxWallKicksActive, DropdownBoxWallKicksEditMode);
	GuiDropdownBox(SettingsRects[0][0][1], DropdownBoxRotationSystemTextList, DropdownBoxRotationSystemActive,
	    DropdownBoxRotationSystemEditMode);

	writeSettings(settingsPresets.getValue());
	settingsPresets.updateState();
	{
		const ScopedGuiLock lock(false);
		settingsPresets.handleSelectionBox(DropdownBoxPresetsRect);
	}
	if(settingsPresets.getPresets().size() < MaxSavedPresets)
	{
		settingsPresets.handleSaveButton(InputSaveRect, "+");
	}
	settingsPresets.handleRemoveButton(InputRemoveRect, "x");
	readSettings(settingsPresets.getValue());
}

void drawEntry(int scoresIdx, const Rectangle& bounds, const App::HighScoreEntry& entry) noexcept
{
	std::array<char, 16> buffer{};
	std::to_chars(&buffer.front(), &buffer.back(), entry.score);
	::GuiLabel({bounds.x + 15, bounds.y + (25 * scoresIdx) + 5, 65, 24}, entry.name.data());
	::GuiLabel({bounds.x + 15 + 70, bounds.y + (25 * scoresIdx) + 5, 65, 24}, buffer.data());
}

template<typename TType, typename TRng, std::enable_if_t<std::is_enum_v<TType>, bool> = true>
TType randomValue(TRng& rng)
{
	constexpr auto values = magic_enum::enum_values<TType>();
	std::uniform_int_distribution<unsigned> dist(0, static_cast<unsigned>(values.size() - 1));
	return static_cast<TType>(values[dist(rng)]);
}
template<typename TType, typename TRng, std::enable_if_t<!std::is_enum_v<TType>, bool> = true>
TType randomValue(
    TRng& rng, TType min = std::numeric_limits<TType>::min(), TType max = std::numeric_limits<TType>::max())
{
	using TDistType = std::conditional_t<std::is_signed_v<TType>, ptrdiff_t, size_t>;
	std::uniform_int_distribution<TDistType> dist(min, max);
	return static_cast<TType>(dist(rng));
}

void genEntries(App& app, int entryCount, const char* namePtr, const App::Settings* setPtr)
{
	std::mt19937_64 rng(std::random_device{}());
	App::Settings settings;
	App::HighScoreEntry::NameT name{};
	for(int i = 0; i < entryCount; ++i)
	{
		if(!namePtr)
		{
			std::uniform_int_distribution<int> charDist(' ', '~');
			std::for_each(name.begin(), name.end(),
			    [&](auto& chr)
			    {
				    chr = static_cast<char>(charDist(rng));
			    });
		}
		if(!setPtr)
		{
			settings.rotationSystem = randomValue<RotationSystem>(rng);
			settings.wallKicks = randomValue<WallKicks>(rng);
			settings.lockDown = randomValue<LockDown>(rng);
			settings.softDrop = randomValue<SoftDrop>(rng);
			settings.instantDrop = randomValue<InstantDrop>(rng);
			settings.tSpin = randomValue<TSpin>(rng);
			settings.shuffleType = randomValue<ShuffleType>(rng);
			settings.scoringSystem = randomValue<ScoringSystem>(rng);
			settings.levelGoal = randomValue<LevelGoal>(rng);
			settings.holdPiece = randomValue<bool>(rng);
			settings.ghostPiece = randomValue<bool>(rng);
			settings.previewCount = randomValue<uint8_t>(rng, 0, 5);
			settings.fieldWidth = randomValue<uint8_t>(rng, 10, 15);
			settings.fieldHeight = randomValue<uint8_t>(rng, 10, 15);
		}
		app.highScores.add(
		    namePtr ? namePtr : name.data(), randomValue<int>(rng, 99, 999999), setPtr ? *setPtr : settings);
	}
	App::storeFile(app.serialize());
}

void drawClose(const Rectangle& bounds, const char* text, App& app, [[maybe_unused]] const char* namePtr,
    [[maybe_unused]] const App::Settings* setPtr, std::function<bool(const App::HighScoreEntry&)> selector)
{
	::GuiGroupBox(bounds, text);
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

void Menu::UpdateDrawHighscores(App& app) noexcept
{
	::GuiGroupBox(GroupBoxSettingsRect, ButtonHighscoresText);
	drawClose(AllScoreRect, "All Scores", app, nullptr, nullptr,
	    [&]([[maybe_unused]] const App::HighScoreEntry& entry)
	    {
		    return true;
	    });
	drawClose(MyScoreRect, "Same Name", app, TextBoxPlayerNameBuffer.data(), nullptr,
	    [&](const App::HighScoreEntry& entry)
	    {
		    return entry.name == TextBoxPlayerNameBuffer;
	    });
	const App::Settings& settings = settingsPresets.getValue();
	drawClose(SetScoreRect, "Same Settings", app, nullptr, &settings,
	    [&](const App::HighScoreEntry& entry)
	    {
		    return entry.settings == settings;
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
		if(myScoresIdx < maxIdx && entry.name == TextBoxPlayerNameBuffer)
		{
			drawEntry(myScoresIdx, MyScoreRect, entry);
			++myScoresIdx;
		}
		if(setScoresIdx < maxIdx && entry.settings == settings)
		{
			drawEntry(setScoresIdx, SetScoreRect, entry);
			++setScoresIdx;
		}
	}
}

void guiKeyBind(const ::Rectangle& rectLabel, const char* textLabel, const ::Rectangle& rectInput,
    Menu::KeyBufferT& keyBuffer, bool& editMode, int16_t& keyBind) noexcept
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

void Menu::UpdateDrawKeyBinds([[maybe_unused]] App& app)
{
	::GuiGroupBox(GroupBoxSettingsRect, ButtonKeyBindsText);
	if(::GuiButton(SettingsRects[1][7][1], GroupBoxSettingsText))
	{
		state = State::Settings;
	}
	guiKeyBind(SettingsRects[1][0][0], LabelMoveRightText, SettingsRects[1][0][1], TextBoxMoveRightBuffer,
	    TextBoxMoveRightEditMode, keyBindsPresets.getValue().moveRight);
	guiKeyBind(SettingsRects[0][0][0], LabelMoveLeftText, SettingsRects[0][0][1], TextBoxMoveLeftBuffer,
	    TextBoxMoveLeftEditMode, keyBindsPresets.getValue().moveLeft);
	guiKeyBind(SettingsRects[1][1][0], LabelRotateRightText, SettingsRects[1][1][1], TextBoxRotateRightBuffer,
	    TextBoxRotateRightEditMode, keyBindsPresets.getValue().rotateRight);
	guiKeyBind(SettingsRects[0][1][0], LabelRotateLeftText, SettingsRects[0][1][1], TextBoxRotateLeftBuffer,
	    TextBoxRotateLeftEditMode, keyBindsPresets.getValue().rotateLeft);
	guiKeyBind(SettingsRects[0][2][0], LabelSoftDropText, SettingsRects[0][2][1], TextBoxSoftDropBuffer,
	    TextBoxSoftDropEditMode, keyBindsPresets.getValue().softDrop);
	guiKeyBind(SettingsRects[1][2][0], LabelHardDropText, SettingsRects[1][2][1], TextBoxHardDropBuffer,
	    TextBoxHardDropEditMode, keyBindsPresets.getValue().hardDrop);
	guiKeyBind(SettingsRects[0][3][0], LabelHoldText, SettingsRects[0][3][1], TextBoxHoldBuffer, TextBoxHoldEditMode,
	    keyBindsPresets.getValue().hold);
	guiKeyBind(SettingsRects[0][4][0], LabelPauseText, SettingsRects[0][4][1], TextBoxPauseBuffer, TextBoxPauseEditMode,
	    keyBindsPresets.getValue().pause);
	guiKeyBind(SettingsRects[1][4][0], LabelRestartText, SettingsRects[1][4][1], TextBoxRestartBuffer,
	    TextBoxRestartEditMode, keyBindsPresets.getValue().restart);
	guiKeyBind(SettingsRects[1][3][0], LabelMenuText, SettingsRects[1][3][1], TextBoxMenuBuffer, TextBoxMenuEditMode,
	    keyBindsPresets.getValue().menu);

	keyBindsPresets.updateState();
	::GuiLabel(LabelPresetsRect, LabelPresetsText);
	{
		const ScopedGuiLock lock(false);
		if(keyBindsPresets.handleSelectionBox(DropdownBoxPresetsRect))
		{
			updateKeyBindBuffers(keyBindsPresets.getValue());
		}
	}
	if(keyBindsPresets.getPresets().size() < MaxSavedPresets)
	{
		keyBindsPresets.handleSaveButton(InputSaveRect, "+");
	}
	keyBindsPresets.handleRemoveButton(InputRemoveRect, "x");
}

void Menu::UpdateDrawAbout([[maybe_unused]] App& app) noexcept
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

	const int prevTextColorProp = ::GuiGetStyle(::GuiControl::LABEL, ::GuiControlProperty::TEXT_COLOR_NORMAL);
	::GuiSetStyle(::GuiControl::LABEL, ::GuiControlProperty::TEXT_COLOR_NORMAL, ColorToInt(BLUE));

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

	::GuiSetStyle(::GuiControl::LABEL, ::GuiControlProperty::TEXT_COLOR_NORMAL, prevTextColorProp);

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
