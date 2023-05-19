#pragma once

#include "scenes.hpp"

#include <raylib.h>

namespace raymino
{
struct Menu : public IScene
{
	Menu();
	void UpdateDraw(class App& app) override;

	const char* GroupBoxGameText = "Game";
	const char* ButtonStartGameText = "Start Game";
	const char* ButtonHighscoresText = "Highscores";
	const char* GroupBoxSettingsText = "Settings";
	const char* LabelRotationSystemText = "Rotation System";
	const char* LabelWallKicksText = "Wall Kicks";
	const char* LabelLockDownText = "Lock Down";
	const char* LabelSoftDropText = "Soft Drop";
	const char* LabelInstantDropText = "Instant Drop";
	const char* LabelTSpinText = "TSpin";
	const char* LabelShuffleTypeText = "Shuffle Type";
	const char* LabelScoringSystemText = "Scoring System";
	const char* LabelFieldWidthText = "Field Width";
	const char* LabelFieldHeightText = "Field Height";
	const char* LabelPreviewCountText = "Preview Count";
	const char* LabelHoldPieceText = "Hold Piece";
	const char* LabelPresetsText = "Presets";
	const char* DropdownBoxRotationSystemText = "Nintendo Right;PlaceHolder";
	const char* DropdownBoxWallKicksText = "Arika;PlaceHolder";
	const char* DropdownBoxLockDownText = "Extended;PlaceHolder";
	const char* DropdownBoxSoftDropText = "None Locking;PlaceHolder";
	const char* DropdownBoxInstantDropText = "Sonic;PlaceHolder";
	const char* DropdownBoxTSpinText = "Three Corner;PlaceHolder";
	const char* DropdownBoxShuffleTypeText = "Single Bag;PlaceHolder";
	const char* DropdownBoxScoringSystemText = "Guideline;PlaceHolder";
	const char* DropdownBoxHoldPieceText = "Yes;PlaceHolder";
	const char* DropdownBoxPresetsText = "Guideline;PlaceHolder";

	const Vector2 AnchorGame = {24, 24};
	const Vector2 AnchorSettings = {24, 120};

	bool DropdownBoxRotationSystemEditMode = false;
	bool DropdownBoxWallKicksEditMode = false;
	bool DropdownBoxLockDownEditMode = false;
	bool DropdownBoxSoftDropEditMode = false;
	bool DropdownBoxInstantDropEditMode = false;
	bool DropdownBoxTSpinEditMode = false;
	bool DropdownBoxShuffleTypeEditMode = false;
	bool DropdownBoxScoringSystemEditMode = false;
	bool DropdownBoxHoldPieceEditMode = false;
	bool SpinnerPreviewCountEditMode = false;
	bool SpinnerFieldWidthEditMode = false;
	bool SpinnerFieldHeightEditMode = false;
	bool DropdownBoxPresetsEditMode = false;
	bool ButtonGameStartClicked = false;
	bool ButtonHighscoresClicked = false;

	int DropdownBoxRotationSystemActive = 0;
	int DropdownBoxWallKicksActive = 0;
	int DropdownBoxLockDownActive = 0;
	int DropdownBoxSoftDropActive = 0;
	int DropdownBoxInstantDropActive = 0;
	int DropdownBoxTSpinActive = 0;
	int DropdownBoxShuffleTypeActive = 0;
	int DropdownBoxScoringSystemActive = 0;
	int DropdownBoxHoldPieceActive = 0;
	int SpinnerPreviewCountValue = 0;
	int SpinnerFieldWidthValue = 0;
	int SpinnerFieldHeightValue = 0;
	int DropdownBoxPresetsActive = 0;

	const Rectangle layoutRecs[30] = {
	    {AnchorGame.x + 0, AnchorGame.y + 0, 552, 72},
	    {AnchorSettings.x + 0, AnchorSettings.y + 0, 552, 456},
	    {AnchorGame.x + 88, AnchorGame.y + 16, 152, 40},
	    {AnchorSettings.x + 24, AnchorSettings.y + 72, 96, 24},
	    {AnchorSettings.x + 24, AnchorSettings.y + 120, 96, 24},
	    {AnchorSettings.x + 24, AnchorSettings.y + 168, 96, 24},
	    {AnchorSettings.x + 24, AnchorSettings.y + 216, 96, 24},
	    {AnchorSettings.x + 24, AnchorSettings.y + 264, 96, 24},
	    {AnchorSettings.x + 24, AnchorSettings.y + 312, 96, 24},
	    {AnchorSettings.x + 24, AnchorSettings.y + 360, 96, 24},
	    {AnchorSettings.x + 24, AnchorSettings.y + 408, 96, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 72, 136, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 120, 136, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 168, 136, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 216, 136, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 264, 136, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 312, 136, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 360, 136, 24},
	    {AnchorSettings.x + 120, AnchorSettings.y + 408, 136, 24},
	    {AnchorSettings.x + 296, AnchorSettings.y + 72, 96, 24},
	    {AnchorSettings.x + 296, AnchorSettings.y + 120, 96, 24},
	    {AnchorSettings.x + 296, AnchorSettings.y + 168, 96, 24},
	    {AnchorSettings.x + 296, AnchorSettings.y + 216, 96, 24},
	    {AnchorSettings.x + 392, AnchorSettings.y + 216, 136, 24},
	    {AnchorSettings.x + 392, AnchorSettings.y + 168, 136, 24},
	    {AnchorSettings.x + 392, AnchorSettings.y + 72, 136, 24},
	    {AnchorSettings.x + 392, AnchorSettings.y + 120, 136, 24},
	    {AnchorGame.x + 312, AnchorGame.y + 16, 152, 40},
	    {AnchorSettings.x + 120, AnchorSettings.y + 16, 272, 32},
	    {AnchorSettings.x + 24, AnchorSettings.y + 16, 96, 24},
	};
};
} // namespace raymino
