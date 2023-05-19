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

Menu::Menu()
{
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
	ButtonGameStartClicked = GuiButton(layoutRecs[2], ButtonStartGameText);
	ButtonHighscoresClicked = GuiButton(layoutRecs[27], ButtonHighscoresText);

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
	if(GuiSpinner(layoutRecs[24], "", &SpinnerPreviewCountValue, 0, 10, SpinnerPreviewCountEditMode))
	{
		SpinnerPreviewCountEditMode = !SpinnerPreviewCountEditMode;
	}
	if(GuiSpinner(layoutRecs[25], "", &SpinnerFieldWidthValue, 5, 100, SpinnerFieldWidthEditMode))
	{
		SpinnerFieldWidthEditMode = !SpinnerFieldWidthEditMode;
	}
	if(GuiSpinner(layoutRecs[26], "", &SpinnerFieldHeightValue, 10, 100, SpinnerFieldHeightEditMode))
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

	if(ButtonGameStartClicked)
	{
		app.QueueSceneSwitch(MakeScene<Scene::Game>(app));
		return;
	}
	if(ButtonHighscoresClicked)
	{
		//! TODO		app.QueueSceneSwitch(MakeScene<Scene::Highscores>(app));
		return;
	}
}
} // namespace raymino
