#include "menu.hpp"

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>

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

void Menu::UpdateDrawHighscores(App& app)
{
	GuiGroupBox(GroupBoxSettingsRect, ButtonHighscoresText);
}

void Menu::UpdateDrawKeyBinds(App& app)
{
	GuiGroupBox(GroupBoxSettingsRect, ButtonKeyBindsText);
}
} // namespace raymino
