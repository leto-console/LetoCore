/**
 * @file UI_Label_new.hpp
 * @date Jun 28, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_TEXT_UI_LABEL_NEW_HPP_
#define INC_UI_TEXT_UI_LABEL_NEW_HPP_

#include "LetoCoreExport.h"

#include <Data/StaticTextView.hpp>
#include <Graphics/RGBColor.hpp>
#include <UI/Base/UI_Object.hpp>

enum class UI_Label_HAlign : uint8_t
{
	LEFT,		// Выравнивание по левому краю
	CENTER,		// Выравнивание по центру
    RIGHT,      // Выравнивание по правому краю
};

enum class UI_Label_VAlign : uint8_t
{
	TOP,		// Выравнивание по верхнему краю
	CENTER,		// Выравнивание по середине
    BOTTOM,     // Выравнивание по нижнему краю
};

/// @brief Text label UI element
class LETO_CORE_EXPORT UI_Label_new : public UI_Object
{
public:
    UI_Label_new(Point2_i size);
	
	bool ObjectOnEvent(const AppEvent& event) override;
	void ObjectDraw(LetoScreen_V1* screen) override;

	/// TODO: Think about std::move operation of &&
    void SetText(StaticTextView text);

	StaticTextView GetText() const;

	// Установить горизонтальное выравнивание
	UI_Label_new& SetHorizonAlignment(UI_Label_HAlign alignment);

	// Установить вертикальное выравнивание
	UI_Label_new& SetVerticalAlignment(UI_Label_VAlign align);

	UI_Label_new& SetFont(const LetoFont_V1* font);
	const LetoFont_V1* GetFont() const;

	UI_Label_new& SetTextColor(LetoColor_V1 color);
	LetoColor_V1 GetTextColor() const;

	UI_Label_new& SetBackroundColor(LetoColor_V1 color);
	LetoColor_V1 GetBackroundColor() const;

	UI_Label_new& SetColorInverse(bool inverse);
	
	UI_Label_new& SetStyle(LetoTextStyle_V1 style);
	LetoTextStyle_V1 GetStyle() const;

	void UpdateOffsets();

private:
	StaticTextView label_text;

	UI_Label_HAlign h_align = UI_Label_HAlign::LEFT;
	UI_Label_VAlign v_align = UI_Label_VAlign::TOP;
	RGBColor label_text_color = WhiteColor;
	RGBColor label_background_color = BlackColor;
	bool label_inverse = false;

	int h_offset = 0;
	int v_offset = 0;

	int text_width = 0;
	int text_height = 0;

	LetoTextStyle_V1 label_style{2, 2};
	const LetoFont_V1* label_font = nullptr;
};

#endif
