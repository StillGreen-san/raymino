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

Menu::Menu(App& app) // NOLINT(*-member-init) handled by readSettings
{
	readSettings(app.settings);
	App::Settings defaultSettings;
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
	DropdownBoxHoldPieceActive = static_cast<int>(settings.holdPiece);
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
	settings.holdPiece = static_cast<bool>(DropdownBoxHoldPieceActive);
	settings.previewCount = SpinnerPreviewCountValue;
	settings.fieldWidth = SpinnerFieldWidthValue;
	settings.fieldHeight = SpinnerFieldHeightValue;
}

void Menu::UpdateDraw(App& app)
{
	App::Settings previous;
	writeSettings(previous);

	BeginDrawing();

	ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

	if(DropdownBoxRotationSystemEditMode || DropdownBoxWallKicksEditMode || DropdownBoxLockDownEditMode ||
	    DropdownBoxSoftDropEditMode || DropdownBoxInstantDropEditMode || DropdownBoxTSpinEditMode ||
	    DropdownBoxShuffleTypeEditMode || DropdownBoxScoringSystemEditMode || DropdownBoxHoldPieceEditMode ||
	    DropdownBoxPresetsEditMode)
	{
		GuiLock();
	}

	GuiGroupBox(GroupBoxGameRect, GroupBoxGameText);
	if(GuiButton(ButtonStartGameRect, ButtonStartGameText))
	{
		writeSettings(app.settings);
		app.QueueSceneSwitch(MakeScene<Scene::Game>(app));
	}
	if(GuiButton(ButtonHighscoresRect, ButtonHighscoresText))
	{
		//! TODO		app.QueueSceneSwitch(MakeScene<Scene::Highscores>(app));
	}

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
	GuiLabel(LabelPresetsRect, LabelPresetsText);
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

	GuiUnlock();

	EndDrawing();
}
} // namespace raymino
