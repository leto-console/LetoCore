/**
 * @file UI_Label.hpp
 * @date Jun 28, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_TEXT_UI_LABEL_HPP_
#define INC_UI_TEXT_UI_LABEL_HPP_

#include "LibrariesExport.h"

#include <SceneManager/ISceneObject.hpp>

#include <Data/StaticTextView.hpp>

enum class LabelHorizonAlignment : uint8_t
{
	LEFT,		// Выравнивание по левому краю
	CENTER,		// Выравнивание по центру
    RIGHT,      // Выравнивание по правому краю
};

enum class LabelVerticalAlignment : uint8_t
{
	TOP,		// Выравнивание по верхнему краю
	CENTER,		// Выравнивание по середине
    BOTTOM,     // Выравнивание по нижнему краю
};

/// @brief Text label UI element
class LIBRARIES_EXPORT UI_Label : public ISceneObject
{
public:
	UI_Label() = default;
    UI_Label(Point2_i pos, Point2_i size);
	
	void Draw(IScreen& screen, Point2_i offset = {}) override;

	bool ProcessInput(const AppEvent& event) override;

	/// TODO: Think about std::move operation of &&
    void SetText(const StaticText32& text);

	StaticTextView GetText() const;

	// Установить горизонтальное выравнивание
	UI_Label& SetHorizonAlignment(LabelHorizonAlignment alignment);

	// Установить вертикальное выравнивание
	UI_Label& SetVerticalAlignment(LabelVerticalAlignment align);

	UI_Label& SetFont(IFont* font);

	UI_Label& SetTextColor(RGBColor color);
	RGBColor GetTextColor() const;

	UI_Label& SetBackroundColor(RGBColor color);
	RGBColor GetBackroundColor() const;

	UI_Label& SetColorInverse(bool inverse);

	void UpdateOffsets();

private:
	StaticText32 label_text;

	LabelHorizonAlignment h_align = LabelHorizonAlignment::LEFT;
	LabelVerticalAlignment v_align = LabelVerticalAlignment::TOP;
	RGBColor label_text_color = WhiteColor;
	RGBColor label_background_color = BlackColor;
	bool label_inverse = false;

	int h_offset = 0;
	int v_offset = 0;

	int text_width = 0;
	int text_height = 0;

	IFont* label_font = nullptr;
};

#endif
