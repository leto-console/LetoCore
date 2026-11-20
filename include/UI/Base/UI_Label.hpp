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
#include <UI/Base/UI_Alignable.hpp>
#include <UI/Base/UI_Alignable.hpp>

NEW_LETO_UI_BEGIN

/// @brief Text label UI element
class LETO_CORE_EXPORT UI_Label : public UI_Alignable
{
public:
	UI_Label(Point2_i size = {}, const LetoAllocator_V1* allocator = nullptr) 
		: UI_Alignable{ size, allocator }
	{ }
	UI_Label(Point2_i size, UI_Object& parent)
		: UI_Alignable{ size, parent }
	{ }
	
	void Setup(
		StaticTextView text,
		const LetoFont_V1* font, 
		LetoColor_V1 text_color, 
		LetoColor_V1 back_color = BlackColor, 
		LetoTextStyle_V1 style = {2, 2});
	
    void SetText(StaticTextView text);
	StaticTextView GetText() const;

	void SetFont(const LetoFont_V1* font);
	const LetoFont_V1* GetFont() const;

	void SetTextColor(LetoColor_V1 color);
	LetoColor_V1 GetTextColor() const;

	void SetBackroundColor(LetoColor_V1 color);
	LetoColor_V1 GetBackroundColor() const;

	void SetColorInverse(bool inverse);
	
	void SetStyle(LetoTextStyle_V1 style);
	LetoTextStyle_V1 GetStyle() const;

protected:
	void ObjectDraw(LetoScreen_V1* screen) override;

	void RefreshAlign() override;

private:
	StaticTextView label_text;

	RGBColor label_text_color = WhiteColor;
	RGBColor label_background_color = BlackColor;
	bool label_inverse = false;

	Point2_i align_offset{};

	int text_width = 0;
	int text_height = 0;

	LetoTextStyle_V1 label_style{2, 2};
	const LetoFont_V1* label_font = nullptr;
};

NEW_LETO_UI_END

#endif
