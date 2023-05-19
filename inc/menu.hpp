#pragma once

#include "scenes.hpp"

#include <array>
#include <raylib.h>

namespace raymino
{
struct Menu : public IScene
{
	void UpdateDraw(class App& app) override;

	static constexpr const char* GroupBoxGameText = "Game";
	static constexpr const char* ButtonStartGameText = "Start Game";
	static constexpr const char* ButtonHighscoresText = "Highscores";
	static constexpr const char* GroupBoxSettingsText = "Settings";
	static constexpr const char* LabelRotationSystemText = "Rotation System";
	static constexpr const char* LabelWallKicksText = "Wall Kicks";
	static constexpr const char* LabelLockDownText = "Lock Down";
	static constexpr const char* LabelSoftDropText = "Soft Drop";
	static constexpr const char* LabelInstantDropText = "Instant Drop";
	static constexpr const char* LabelTSpinText = "TSpin";
	static constexpr const char* LabelShuffleTypeText = "Shuffle Type";
	static constexpr const char* LabelScoringSystemText = "Scoring System";
	static constexpr const char* LabelFieldWidthText = "Field Width";
	static constexpr const char* LabelFieldHeightText = "Field Height";
	static constexpr const char* LabelPreviewCountText = "Preview Count";
	static constexpr const char* LabelHoldPieceText = "Hold Piece";
	static constexpr const char* LabelPresetsText = "Presets";
	static constexpr const char* DropdownBoxRotationSystemText =
	    "Original;Super;Arika;Sega;Nintendo Left;Nintendo Right";
	static constexpr const char* DropdownBoxWallKicksText = "None;Arika;Super";
	static constexpr const char* DropdownBoxLockDownText = "Infinit;Extended;Classic;Entry";
	static constexpr const char* DropdownBoxSoftDropText = "Locking;None Locking";
	static constexpr const char* DropdownBoxInstantDropText = "Hard;Sonic";
	static constexpr const char* DropdownBoxTSpinText = "Immobile;Three Corner;Lenient";
	static constexpr const char* DropdownBoxShuffleTypeText = "Random;Single Bag;Double Bag;Triple Bag";
	static constexpr const char* DropdownBoxScoringSystemText = "Guideline;BPS;Sega;Nintendo";
	static constexpr const char* DropdownBoxHoldPieceText = "Yes;No";
	static constexpr const char* DropdownBoxPresetsText = "Guideline";

	static constexpr Vector2 AnchorGame = {24, 24};
	static constexpr Vector2 AnchorSettings = {24, 120};

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

	int DropdownBoxRotationSystemActive = 1;
	int DropdownBoxWallKicksActive = 2;
	int DropdownBoxLockDownActive = 2;
	int DropdownBoxSoftDropActive = 1;
	int DropdownBoxInstantDropActive = 0;
	int DropdownBoxTSpinActive = 1;
	int DropdownBoxShuffleTypeActive = 1;
	int DropdownBoxScoringSystemActive = 0;
	int DropdownBoxHoldPieceActive = 0;
	static constexpr int SpinnerPreviewCountMin = 0;
	int SpinnerPreviewCountValue = 6;
	static constexpr int SpinnerPreviewCountMax = 10;
	static constexpr int SpinnerFieldWidthMin = 5;
	int SpinnerFieldWidthValue = 10;
	static constexpr int SpinnerFieldWidthMax = 50;
	static constexpr int SpinnerFieldHeightMin = 10;
	int SpinnerFieldHeightValue = 20;
	static constexpr int SpinnerFieldHeightMax = 50;
	int DropdownBoxPresetsActive = 0;

	static constexpr std::array<Rectangle, 30> layoutRecs{{
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
	}};
};
} // namespace raymino
