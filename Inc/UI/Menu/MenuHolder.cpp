#include <UI/Menu/MenuHolder.hpp>

#include <DrawFunctions/DrawText.hpp>
#include <Input/ButtonEvent.hpp>

MenuHolder::MenuHolder(StaticListView<StaticText32> texts, uint8_t visible_elements, Point2_i position)
	: texts{ texts }, visible_elements{ visible_elements }, 
	up_catcher{ ButtonEvent::Compare, this, &MenuHolder::Up },
	down_catcher{ ButtonEvent::Compare, this, &MenuHolder::Down }
{
	SetPosition(position);
}

void MenuHolder::RegUpEvent(const AppEvent & event)
{
	up_catcher.Catch(event);
}

void MenuHolder::RegDownEvent(const AppEvent & event)
{
	down_catcher.Catch(event);
}

void MenuHolder::OnShow()
{
	if (reset_on_show) 
		ResetCurrentID();
}

void MenuHolder::SetStyle(MenuStyle style, IFont* font)
{
	this->style = style;
	if (font)
	{
		this->font = font;
		CharWidth = font->GetWidth();
		CharHeight = font->GetHeight();
	}
	ResetCurrentID();
}

void MenuHolder::SetResetOnShow(bool reset)
{
	reset_on_show = reset;
}

void MenuHolder::SetHorizonAlignment(MenuHorizonAlignment align)
{
	this->horizon_align = align;
	ResetCurrentID();
}

void MenuHolder::SetVerticalAlignment(MenuVerticalAlignment align)
{
	this->vertical_align = align;
	ResetCurrentID();
}

StaticText32 MenuHolder::GetText(uint8_t ID) const
{
	if (ID >= texts.Count())
		return texts.Back();
	return texts[ID];
}

uint8_t MenuHolder::Count() const
{
	return texts.Count();
}

void MenuHolder::ResetCurrentID()
{
	currentID = topID = 0;
	if (vertical_align == MenuVerticalAlignment::CENTER)
	{
		topID = currentID - visible_elements / 2;
	}
}

void MenuHolder::Up()
{
	if (currentID == 0) return;
	currentID--;

	if (vertical_align == MenuVerticalAlignment::CENTER)
	{
		topID = currentID - visible_elements / 2;
	}
	else
	{
		if (currentID < topID) topID = currentID;
	}
}

void MenuHolder::Down()
{
	if (currentID >= Count() - 1) return;
	currentID++;

	if (vertical_align == MenuVerticalAlignment::CENTER)
	{
		topID = currentID - visible_elements / 2;
	}
	else
	{
		if (currentID >= topID + visible_elements) topID = currentID - visible_elements + 1;
	}
}

void MenuHolder::Draw(IScreen& screen, Point2_i offset)
{
	using namespace DrawFunctions;

	for (int i = topID; i < Count() && i < topID + visible_elements; i++)
	{
		if (i < 0)
			continue;

		int elem_offset_x = position.x + offset.x;
		int elem_offset_y = position.y + offset.y + (i - topID) * CharHeight;

		if (vertical_align == MenuVerticalAlignment::CENTER)
		{
			elem_offset_y -= (visible_elements / 2) * CharHeight;
		}

		if (horizon_align == MenuHorizonAlignment::CENTER && style == MenuStyle::STYLE_3)
		{
			elem_offset_x -= (TextWidth(texts[i], font) / 2);
		}

		DrawText(
			screen,
			{style == MenuStyle::STYLE_3 ? elem_offset_x : CharWidth + elem_offset_x, elem_offset_y},
			texts[i],
			WhiteColor, BlackColor,
			style == MenuStyle::STYLE_1 ? false : i == currentID, 
			font);
	}

	static StaticText<8> symbol = ">";

	if (style != MenuStyle::STYLE_3 && Count() > 0)
	{
		if (vertical_align == MenuVerticalAlignment::CENTER)
		{
			DrawText(
				screen,
				position + offset,
				symbol,
				WhiteColor, BlackColor,
				false,
				font);
			return;
		}
		
		int id = currentID - topID;

		if (id >= 0 || id < visible_elements)
		{
			DrawText(
				screen,
				position + offset + Point2_i{0, id*CharHeight},
				symbol,
				WhiteColor, BlackColor,
				false,
				font);
		}
	}
}

#include <Input/SystemInputID.hpp>
#include "MenuHolder.hpp"

bool MenuHolder::ProcessInput(const AppEvent& event)
{	
	if (up_catcher.ProcessInput(event) ||
		down_catcher.ProcessInput(event))
		return true;

	if (IsSystemPrevEvent(event, true))
	{
		Up();
		return true;
	}
	else if (IsSystemNextEvent(event, true))
	{
		Down();
		return true;
	}

	return false;
}
