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

Menu::Menu(App& app) :
    DropdownBoxRotationSystemActive{static_cast<int>(app.settings.rotationSystem)},
    DropdownBoxWallKicksActive{static_cast<int>(app.settings.wallKicks)},
    DropdownBoxLockDownActive{static_cast<int>(app.settings.lockDown)},
    DropdownBoxSoftDropActive{static_cast<int>(app.settings.softDrop)},
    DropdownBoxInstantDropActive{static_cast<int>(app.settings.instantDrop)},
    DropdownBoxTSpinActive{static_cast<int>(app.settings.tSpin)},
    DropdownBoxShuffleTypeActive{static_cast<int>(app.settings.shuffleType)},
    DropdownBoxScoringSystemActive{static_cast<int>(app.settings.scoringSystem)},
    DropdownBoxHoldPieceActive{static_cast<int>(app.settings.holdPiece)},
    SpinnerPreviewCountValue{app.settings.previewCount}, SpinnerFieldWidthValue{app.settings.fieldWidth},
    SpinnerFieldHeightValue{app.settings.fieldHeight}
{
}

void Menu::updateSettings(App& app) const
{
	app.settings.rotationSystem = static_cast<RotationSystem>(DropdownBoxRotationSystemActive);
	app.settings.wallKicks = static_cast<WallKicks>(DropdownBoxWallKicksActive);
	app.settings.lockDown = static_cast<LockDown>(DropdownBoxLockDownActive);
	app.settings.softDrop = static_cast<SoftDrop>(DropdownBoxSoftDropActive);
	app.settings.instantDrop = static_cast<InstantDrop>(DropdownBoxInstantDropActive);
	app.settings.tSpin = static_cast<TSpin>(DropdownBoxTSpinActive);
	app.settings.shuffleType = static_cast<ShuffleType>(DropdownBoxShuffleTypeActive);
	app.settings.scoringSystem = static_cast<ScoringSystem>(DropdownBoxScoringSystemActive);
	app.settings.holdPiece = static_cast<bool>(DropdownBoxHoldPieceActive);
	app.settings.previewCount = SpinnerPreviewCountValue;
	app.settings.fieldWidth = SpinnerFieldWidthValue;
	app.settings.fieldHeight = SpinnerFieldHeightValue;
}

void Menu::UpdateDraw(App& app)
{
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
		updateSettings(app);
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
	if(GuiDropdownBox(
	       DropdownBoxPresetsRect, DropdownBoxPresetsText, &DropdownBoxPresetsActive, DropdownBoxPresetsEditMode))
	{
		DropdownBoxPresetsEditMode = !DropdownBoxPresetsEditMode;
	}

	GuiUnlock();

	EndDrawing();
}
} // namespace raymino
