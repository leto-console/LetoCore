/*
 * Label.hpp
 *
 *  Created on: Jun 28, 2026
 *      Author: Rakhimov T.
 */

#ifndef INC_UI_LABEL_HPP_
#define INC_UI_LABEL_HPP_

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
class Label : public ISceneObject
{
public:
	Label() = default;
    Label(Point2_i pos, Point2_i size);
	
	void Draw(IScreen& screen, Point2_i offset = {}) override;

	bool ProcessInput(const AppEvent& event) override;

	/// TODO: Think about std::move operation of &&
    void SetText(const StaticText32& text);

	StaticTextView GetText() const;

	// Установить горизонтальное выравнивание
	Label& SetHorizonAlignment(LabelHorizonAlignment alignment);

	// Установить вертикальное выравнивание
	Label& SetVerticalAlignment(LabelVerticalAlignment align);

	Label& SetFont(IFont* font);

	Label& SetTextColor(RGBColor color);

	Label& SetBackroundColor(RGBColor color);

	Label& SetColorInverse(bool inverse);

private:
	LabelHorizonAlignment h_align = LabelHorizonAlignment::LEFT;
	LabelVerticalAlignment v_align = LabelVerticalAlignment::TOP;

	int h_offset = 0;
	int v_offset = 0;

	void UpdateOffsets();

	StaticText32 label_text;

	int text_width = 0;
	int text_height = 0;

	IFont* label_font = nullptr;
	RGBColor label_text_color = WhiteColor;
	RGBColor label_background_color = BlackColor;
	bool label_inverse = false;
};

#endif
