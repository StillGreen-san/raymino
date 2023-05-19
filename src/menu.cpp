#include "menu.hpp"

#include "app.hpp"

#include <raygui.h>
#include <raylib-cpp.hpp>

namespace raymino
{
template<>
std::unique_ptr<IScene> MakeScene<Scene::Menu>([[maybe_unused]] App& app)
{
	return std::make_unique<Menu>();
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

	GuiGroupBox(layoutRecs[0], GroupBoxGameText);
	if(GuiButton(layoutRecs[2], ButtonStartGameText))
	{
		app.QueueSceneSwitch(MakeScene<Scene::Game>(app));
	}
	if(GuiButton(layoutRecs[27], ButtonHighscoresText))
	{
		//! TODO		app.QueueSceneSwitch(MakeScene<Scene::Highscores>(app));
	}

	GuiGroupBox(layoutRecs[1], GroupBoxSettingsText);
	GuiLabel(layoutRecs[3], LabelRotationSystemText);
	GuiLabel(layoutRecs[4], LabelWallKicksText);
	GuiLabel(layoutRecs[5], LabelLockDownText);
	GuiLabel(layoutRecs[6], LabelSoftDropText);
	GuiLabel(layoutRecs[7], LabelInstantDropText);
	GuiLabel(layoutRecs[8], LabelTSpinText);
	GuiLabel(layoutRecs[9], LabelShuffleTypeText);
	GuiLabel(layoutRecs[10], LabelScoringSystemText);
	GuiLabel(layoutRecs[19], LabelFieldWidthText);
	GuiLabel(layoutRecs[20], LabelFieldHeightText);
	GuiLabel(layoutRecs[21], LabelPreviewCountText);
	GuiLabel(layoutRecs[22], LabelHoldPieceText);
	GuiLabel(layoutRecs[29], LabelPresetsText);
	if(GuiSpinner(layoutRecs[24], "", &SpinnerPreviewCountValue, SpinnerPreviewCountMin, SpinnerPreviewCountMax,
	       SpinnerPreviewCountEditMode))
	{
		SpinnerPreviewCountEditMode = !SpinnerPreviewCountEditMode;
	}
	if(GuiSpinner(layoutRecs[25], "", &SpinnerFieldWidthValue, SpinnerFieldWidthMin, SpinnerFieldWidthMax,
	       SpinnerFieldWidthEditMode))
	{
		SpinnerFieldWidthEditMode = !SpinnerFieldWidthEditMode;
	}
	if(GuiSpinner(layoutRecs[26], "", &SpinnerFieldHeightValue, SpinnerFieldHeightMin, SpinnerFieldHeightMax,
	       SpinnerFieldHeightEditMode))
	{
		SpinnerFieldHeightEditMode = !SpinnerFieldHeightEditMode;
	}
	if(GuiDropdownBox(
	       layoutRecs[23], DropdownBoxHoldPieceText, &DropdownBoxHoldPieceActive, DropdownBoxHoldPieceEditMode))
	{
		DropdownBoxHoldPieceEditMode = !DropdownBoxHoldPieceEditMode;
	}
	if(GuiDropdownBox(layoutRecs[18], DropdownBoxScoringSystemText, &DropdownBoxScoringSystemActive,
	       DropdownBoxScoringSystemEditMode))
	{
		DropdownBoxScoringSystemEditMode = !DropdownBoxScoringSystemEditMode;
	}
	if(GuiDropdownBox(
	       layoutRecs[17], DropdownBoxShuffleTypeText, &DropdownBoxShuffleTypeActive, DropdownBoxShuffleTypeEditMode))
	{
		DropdownBoxShuffleTypeEditMode = !DropdownBoxShuffleTypeEditMode;
	}
	if(GuiDropdownBox(layoutRecs[16], DropdownBoxTSpinText, &DropdownBoxTSpinActive, DropdownBoxTSpinEditMode))
	{
		DropdownBoxTSpinEditMode = !DropdownBoxTSpinEditMode;
	}
	if(GuiDropdownBox(
	       layoutRecs[15], DropdownBoxInstantDropText, &DropdownBoxInstantDropActive, DropdownBoxInstantDropEditMode))
	{
		DropdownBoxInstantDropEditMode = !DropdownBoxInstantDropEditMode;
	}
	if(GuiDropdownBox(layoutRecs[14], DropdownBoxSoftDropText, &DropdownBoxSoftDropActive, DropdownBoxSoftDropEditMode))
	{
		DropdownBoxSoftDropEditMode = !DropdownBoxSoftDropEditMode;
	}
	if(GuiDropdownBox(layoutRecs[13], DropdownBoxLockDownText, &DropdownBoxLockDownActive, DropdownBoxLockDownEditMode))
	{
		DropdownBoxLockDownEditMode = !DropdownBoxLockDownEditMode;
	}
	if(GuiDropdownBox(
	       layoutRecs[12], DropdownBoxWallKicksText, &DropdownBoxWallKicksActive, DropdownBoxWallKicksEditMode))
	{
		DropdownBoxWallKicksEditMode = !DropdownBoxWallKicksEditMode;
	}
	if(GuiDropdownBox(layoutRecs[11], DropdownBoxRotationSystemText, &DropdownBoxRotationSystemActive,
	       DropdownBoxRotationSystemEditMode))
	{
		DropdownBoxRotationSystemEditMode = !DropdownBoxRotationSystemEditMode;
	}
	if(GuiDropdownBox(layoutRecs[28], DropdownBoxPresetsText, &DropdownBoxPresetsActive, DropdownBoxPresetsEditMode))
	{
		DropdownBoxPresetsEditMode = !DropdownBoxPresetsEditMode;
	}

	GuiUnlock();

	EndDrawing();
}
} // namespace raymino
