#pragma once

#include "app.hpp"
#include "scenes.hpp"

#include <array>
#include <raylib.h>

namespace raymino
{
struct Menu : public IScene
{
	Menu(App& app);

	void UpdateDraw(App& app) override;
	void readSettings(const App::Settings& settings);
	void writeSettings(App::Settings& settings) const;

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
	static constexpr const char* LabelLevelGoalText = "Level Goal";
	static constexpr const char* LabelFieldWidthText = "Field Width";
	static constexpr const char* LabelFieldHeightText = "Field Height";
	static constexpr const char* LabelPreviewCountText = "Preview Count";
	static constexpr const char* LabelHoldPieceText = "Hold Piece";
	static constexpr const char* LabelGhostPieceText = "Ghost Piece";
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
	static constexpr const char* DropdownBoxLevelGoalText = "Fixed;Dynamic";
	static constexpr const char* DropdownBoxHoldPieceText = "No;Yes";
	static constexpr const char* DropdownBoxGhostPieceText = "No;Yes";
	static constexpr const char* DropdownBoxPresetsText = "Custom;Guideline";

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
	bool DropdownBoxLevelGoalEditMode = false;
	bool DropdownBoxHoldPieceEditMode = false;
	bool DropdownBoxGhostPieceEditMode = false;
	bool SpinnerPreviewCountEditMode = false;
	bool SpinnerFieldWidthEditMode = false;
	bool SpinnerFieldHeightEditMode = false;
	bool DropdownBoxPresetsEditMode = false;
	bool TextBoxPlayerNameEditMode = false;

	int DropdownBoxRotationSystemActive;
	int DropdownBoxWallKicksActive;
	int DropdownBoxLockDownActive;
	int DropdownBoxSoftDropActive;
	int DropdownBoxInstantDropActive;
	int DropdownBoxTSpinActive;
	int DropdownBoxShuffleTypeActive;
	int DropdownBoxLevelGoalActive;
	int DropdownBoxScoringSystemActive;
	int DropdownBoxHoldPieceActive;
	int DropdownBoxGhostPieceActive;
	static constexpr int SpinnerPreviewCountMin = 0;
	int SpinnerPreviewCountValue;
	static constexpr int SpinnerPreviewCountMax = 10;
	static constexpr int SpinnerFieldWidthMin = 5;
	int SpinnerFieldWidthValue;
	static constexpr int SpinnerFieldWidthMax = 22;
	static constexpr int SpinnerFieldHeightMin = 10;
	int SpinnerFieldHeightValue;
	static constexpr int SpinnerFieldHeightMax = 45;
	int DropdownBoxPresetsActive = 1;

	App::HighScoreEntry::NameT TextBoxPlayerNameBuffer{};

	static constexpr Rectangle GroupBoxGameRect{AnchorGame.x + 0, AnchorGame.y + 0, 552, 72};
	static constexpr Rectangle ButtonStartGameRect{AnchorGame.x + 34, AnchorGame.y + 16, 152, 40};
	static constexpr Rectangle LabelPlayerNameRect{AnchorGame.x + 218, AnchorGame.y + 16, 152, 16};
	static constexpr Rectangle TextBoxPlayerNameRect{AnchorGame.x + 218, AnchorGame.y + 30, 116, 24};
	static constexpr Rectangle ButtonHighscoresRect{AnchorGame.x + 366, AnchorGame.y + 16, 152, 40};
	static constexpr Rectangle GroupBoxSettingsRect{AnchorSettings.x + 0, AnchorSettings.y + 0, 552, 456};
	static constexpr Rectangle LabelRotationSystemRect{AnchorSettings.x + 24, AnchorSettings.y + 72, 96, 24};
	static constexpr Rectangle LabelWallKicksRect{AnchorSettings.x + 24, AnchorSettings.y + 120, 96, 24};
	static constexpr Rectangle LabelLockDownRect{AnchorSettings.x + 24, AnchorSettings.y + 168, 96, 24};
	static constexpr Rectangle LabelSoftDropRect{AnchorSettings.x + 24, AnchorSettings.y + 216, 96, 24};
	static constexpr Rectangle LabelInstantDropRect{AnchorSettings.x + 24, AnchorSettings.y + 264, 96, 24};
	static constexpr Rectangle LabelTSpinRect{AnchorSettings.x + 296, AnchorSettings.y + 72, 96, 24};
	static constexpr Rectangle LabelShuffleTypeRect{AnchorSettings.x + 296, AnchorSettings.y + 120, 96, 24};
	static constexpr Rectangle LabelScoringSystemRect{AnchorSettings.x + 296, AnchorSettings.y + 168, 96, 24};
	static constexpr Rectangle DropdownBoxRotationSystemRect{AnchorSettings.x + 120, AnchorSettings.y + 72, 136, 24};
	static constexpr Rectangle DropdownBoxWallKicksRect{AnchorSettings.x + 120, AnchorSettings.y + 120, 136, 24};
	static constexpr Rectangle DropdownBoxLockDownRect{AnchorSettings.x + 120, AnchorSettings.y + 168, 136, 24};
	static constexpr Rectangle DropdownBoxSoftDropRect{AnchorSettings.x + 120, AnchorSettings.y + 216, 136, 24};
	static constexpr Rectangle DropdownBoxInstantDropRect{AnchorSettings.x + 120, AnchorSettings.y + 264, 136, 24};
	static constexpr Rectangle DropdownBoxTSpinRect{AnchorSettings.x + 392, AnchorSettings.y + 72, 136, 24};
	static constexpr Rectangle DropdownBoxShuffleTypeRect{AnchorSettings.x + 392, AnchorSettings.y + 120, 136, 24};
	static constexpr Rectangle DropdownBoxScoringSystemRect{AnchorSettings.x + 392, AnchorSettings.y + 168, 136, 24};
	static constexpr Rectangle LabelFieldWidthRect{AnchorSettings.x + 24, AnchorSettings.y + 312, 96, 24};
	static constexpr Rectangle LabelFieldHeightRect{AnchorSettings.x + 24, AnchorSettings.y + 360, 96, 24};
	static constexpr Rectangle LabelPreviewCountRect{AnchorSettings.x + 24, AnchorSettings.y + 408, 96, 24};
	static constexpr Rectangle LabelHoldPieceRect{AnchorSettings.x + 296, AnchorSettings.y + 216, 96, 24};
	static constexpr Rectangle DropdownBoxHoldPieceRect{AnchorSettings.x + 392, AnchorSettings.y + 216, 136, 24};
	static constexpr Rectangle LabelLevelGoalRect{AnchorSettings.x + 296, AnchorSettings.y + 264, 96, 24};
	static constexpr Rectangle DropdownBoxLevelGoalRect{AnchorSettings.x + 392, AnchorSettings.y + 264, 136, 24};
	static constexpr Rectangle LabelGhostPieceRect{AnchorSettings.x + 296, AnchorSettings.y + 312, 96, 24};
	static constexpr Rectangle DropdownBoxGhostPieceRect{AnchorSettings.x + 392, AnchorSettings.y + 312, 136, 24};
	static constexpr Rectangle SpinnerPreviewCountRect{AnchorSettings.x + 120, AnchorSettings.y + 408, 136, 24};
	static constexpr Rectangle SpinnerFieldWidthRect{AnchorSettings.x + 120, AnchorSettings.y + 312, 136, 24};
	static constexpr Rectangle SpinnerFieldHeightRect{AnchorSettings.x + 120, AnchorSettings.y + 360, 136, 24};
	static constexpr Rectangle DropdownBoxPresetsRect{AnchorSettings.x + 120, AnchorSettings.y + 16, 272, 32};
	static constexpr Rectangle LabelPresetsRect{AnchorSettings.x + 24, AnchorSettings.y + 16, 96, 24};
};
} // namespace raymino
