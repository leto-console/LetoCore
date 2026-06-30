#include <UI/Menu/MenuHolder.hpp>

#include <DrawFunctions/DrawText.hpp>
#include <Input/ButtonEvent.hpp>

#include <Input/SystemInputID.hpp>

MenuHolder::MenuHolder(StaticListView<StaticText32> texts, uint8_t visible_elements, Point2_i position, bool ready_logic)
	: texts{ texts }, visible_elements{ visible_elements }, 
	up_catcher{ this, &MenuHolder::Up },
	down_catcher{ this, &MenuHolder::Down },
	enter_cather{ this, &MenuHolder::Enter },
	ready_logic{ ready_logic }
{
	SetPosition(position);
}

void MenuHolder::InitBaseCatchers()
{
	RegUpEvent(&IsSystemPrevEvent);
    RegDownEvent(&IsSystemNextEvent);
	RegEnterEvent(&IsSystemEnterEvent);
}

void MenuHolder::EnableReadyLogic()
{
	ready_logic = true;
}

void MenuHolder::RegUpEvent(IsEventFunc is_event)
{
	up_catcher.Catch(is_event);
}

void MenuHolder::RegDownEvent(IsEventFunc is_event)
{
	down_catcher.Catch(is_event);
}

void MenuHolder::RegEnterEvent(IsEventFunc is_event)
{
	enter_cather.Catch(is_event);
}

void MenuHolder::OnShow()
{
	if (reset_on_show) 
		ResetCurrentID();
}

void MenuHolder::SetStyle(MenuStyle style, const IFont* font)
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
	ready = false;
	if (vertical_align == MenuVerticalAlignment::CENTER)
	{
		topID = currentID - visible_elements / 2;
	}
}

bool MenuHolder::IsResultReady(int &idx) const
{
	if (!ready_logic || !IDrawable::IsActive() || !ready)
		return false;
	idx = currentID;
	return true;
}

void MenuHolder::SubmitReady()
{
	ready = false;
}

void MenuHolder::Up()
{
	if (ready_logic && ready || currentID == 0) return;
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
	if (ready_logic && ready || currentID >= Count() - 1) return;
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

void MenuHolder::Enter()
{
	ready = true;
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
	if (ready_logic && ready) return true;

	if (up_catcher.ProcessInput(event) ||
		down_catcher.ProcessInput(event) ||
		ready_logic && enter_cather.ProcessInput(event))
		return true;

	return false;
}
