#pragma once

#include "app.hpp"
#include "gui.hpp"
#include "scenes.hpp"

#include <array>
#include <raylib.h>

namespace raymino
{
template<size_t TRows, size_t TCols>
using RectArr = std::array<std::array<std::array<::Rectangle, 2>, TRows>, TCols>;
template<size_t TRows, size_t TCols>
constexpr RectArr<TRows, TCols> makeSettingsRectArr(const ::Rectangle area, const Box padding, const float itemHeight,
    const float labelWidth, const float columnSpacing) noexcept
{
	const ::Rectangle contentArea{area.x + padding.left, area.y + padding.top,
	    area.width - (padding.left + padding.right), area.height - (padding.top + padding.bottom)};
	const float columnWidth = (contentArea.width - (columnSpacing * (TCols - 1))) / TCols;
	const float inputWidth = columnWidth - labelWidth;
	const float rowSpacing = (contentArea.height - (TRows * itemHeight)) / (TRows - 1);

	RectArr<TRows, TCols> rects{};

	float colX = contentArea.x;
	for(size_t col = 0; col < TCols; ++col)
	{
		float rowY = contentArea.y;
		for(size_t row = 0; row < TRows; ++row)
		{
			rects[col][row][0] = ::Rectangle{colX, rowY, labelWidth, itemHeight};
			rects[col][row][1] = ::Rectangle{colX + labelWidth, rowY, inputWidth, itemHeight};
			rowY += itemHeight + rowSpacing;
		}
		colX += columnSpacing + columnWidth;
	}

	return rects;
}

struct Menu : public IScene
{
	explicit Menu(App& app);

	void UpdateDraw(App& app) override;
	void PreDestruct(App& app) override;

	void readSettings(const App::Settings& settings) noexcept;
	void updateKeyBindBuffers(const App::KeyBinds& keyBinds) noexcept;
	void writeSettings(App::Settings& settings) const noexcept;

	void UpdateDrawSettings(App& app);
	void UpdateDrawHighscores(App& app) noexcept;
	void UpdateDrawKeyBinds(App& app);
	void UpdateDrawAbout(App& app) noexcept;

	enum class State
	{
		Settings,
		HighScores,
		KeyBinds,
	};
	State state = State::Settings;

	static constexpr const char* GroupBoxGameText = "Game";
	static constexpr const char* ButtonStartGameText = "Start Game";
	static constexpr const char* ButtonHighscoresText = "Highscores";
	static constexpr const char* ButtonKeyBindsText = "Key Binds";
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
	static constexpr const char* LabelMoveRightText = "Move Right";
	static constexpr const char* LabelMoveLeftText = "Move Left";
	static constexpr const char* LabelRotateRightText = "Rotate Right";
	static constexpr const char* LabelRotateLeftText = "Rotate Left";
	static constexpr const char* LabelHardDropText = "Hard Drop";
	static constexpr const char* LabelHoldText = "Hold";
	static constexpr const char* LabelPauseText = "Pause";
	static constexpr const char* LabelRestartText = "Restart";
	static constexpr const char* LabelMenuText = "Menu";
	static constexpr const char* LabelSeedText = "Seed";
	TextList DropdownBoxRotationSystemTextList;
	TextList DropdownBoxWallKicksTextList;
	TextList DropdownBoxLockDownTextList;
	TextList DropdownBoxSoftDropTextList;
	TextList DropdownBoxInstantDropTextList;
	TextList DropdownBoxTSpinTextList;
	TextList DropdownBoxShuffleTypeTextList;
	TextList DropdownBoxScoringSystemTextList;
	TextList DropdownBoxLevelGoalTextList;
	static constexpr const char* DropdownBoxHoldPieceText = "No;Yes";
	static constexpr const char* DropdownBoxGhostPieceText = "No;Yes";

	static constexpr ::Vector2 AnchorGame = {24, 24};
	static constexpr ::Vector2 AnchorSettings = {24, 120};

	static constexpr int TextFontSize = 10;
	static constexpr int HeadingFontSize = 20;
	static constexpr size_t MaxSavedPresets = std::min<size_t>(App::MAX_PRESETS, 12);

	bool AboutDialogShowing = false;
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
	bool TextBoxPlayerNameEditMode = false;
	bool TextBoxMoveRightEditMode = false;
	bool TextBoxMoveLeftEditMode = false;
	bool TextBoxRotateRightEditMode = false;
	bool TextBoxRotateLeftEditMode = false;
	bool TextBoxSoftDropEditMode = false;
	bool TextBoxHardDropEditMode = false;
	bool TextBoxHoldEditMode = false;
	bool TextBoxPauseEditMode = false;
	bool TextBoxRestartEditMode = false;
	bool TextBoxMenuEditMode = false;
	bool TextBoxSeedEditMode = false;

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

	App::HighScoreEntry::NameT TextBoxPlayerNameBuffer{};
	using KeyBufferT = std::array<char, 20>;
	KeyBufferT TextBoxMoveRightBuffer{};
	KeyBufferT TextBoxMoveLeftBuffer{};
	KeyBufferT TextBoxRotateRightBuffer{};
	KeyBufferT TextBoxRotateLeftBuffer{};
	KeyBufferT TextBoxSoftDropBuffer{};
	KeyBufferT TextBoxHardDropBuffer{};
	KeyBufferT TextBoxHoldBuffer{};
	KeyBufferT TextBoxPauseBuffer{};
	KeyBufferT TextBoxRestartBuffer{};
	KeyBufferT TextBoxMenuBuffer{};
	decltype(App::seed) TextBoxSeedBuffer{};

	PresetSelect<App::KeyBinds> keyBindsPresets;
	PresetSelect<App::Settings> settingsPresets;

	static constexpr ::Rectangle GroupBoxGameRect{AnchorGame.x + 0, AnchorGame.y + 0, 552, 72};
	static constexpr ::Rectangle ButtonStartGameRect{AnchorGame.x + 34, AnchorGame.y + 16, 152, 40};
	static constexpr ::Rectangle LabelPlayerNameRect{AnchorGame.x + 218, AnchorGame.y + 16, 152, 16};
	static constexpr ::Rectangle TextBoxPlayerNameRect{AnchorGame.x + 218, AnchorGame.y + 30, 116, 24};
	static constexpr ::Rectangle ButtonHighscoresRect{AnchorGame.x + 366, AnchorGame.y + 16, 152, 40};
	static constexpr ::Rectangle GroupBoxSettingsRect{AnchorSettings.x + 0, AnchorSettings.y + 0, 552, 456};
	static constexpr ::Rectangle LabelPresetsRect{AnchorSettings.x + 24, AnchorSettings.y + 16, 96, 24};
	static constexpr ::Rectangle DropdownBoxPresetsRect{AnchorSettings.x + 120, AnchorSettings.y + 16, 272, 31};
	static constexpr ::Rectangle InputSaveRect{AnchorSettings.x + 404, AnchorSettings.y + 22, 20, 20};
	static constexpr ::Rectangle InputRemoveRect{AnchorSettings.x + 432, AnchorSettings.y + 22, 20, 20};
	static constexpr decltype(auto) SettingsRects =
	    makeSettingsRectArr<8, 2>(GroupBoxSettingsRect, Box{72, 24, 24}, 24, 96, 40);

	static constexpr float ScoreListWidth = (GroupBoxSettingsRect.width - 40) / 3;
	static constexpr ::Rectangle AllScoreRect{
	    GroupBoxSettingsRect.x + 10, GroupBoxSettingsRect.y + 15, ScoreListWidth, GroupBoxSettingsRect.height - 25};
	static constexpr ::Rectangle MyScoreRect{GroupBoxSettingsRect.x + 20 + ScoreListWidth, GroupBoxSettingsRect.y + 15,
	    ScoreListWidth, GroupBoxSettingsRect.height - 25};
	static constexpr ::Rectangle SetScoreRect{GroupBoxSettingsRect.x + 30 + (ScoreListWidth * 2),
	    GroupBoxSettingsRect.y + 15, ScoreListWidth, GroupBoxSettingsRect.height - 25};
};
} // namespace raymino
