#include <UI/Menu/MenuHolder.hpp>

#include <DrawFunctions/DrawText.hpp>
#include <Input/ButtonEvent.hpp>

#include <Input/SystemInputID.hpp>

MenuHolder::MenuHolder(uint8_t visible_elements, Point2_i position, bool ready_logic)
	: visible_elements{ visible_elements }, 
	up_catcher{ this, &MenuHolder::Up },
	down_catcher{ this, &MenuHolder::Down },
	enter_cather{ this, &MenuHolder::Enter },
	enc_catcher{ this, &MenuHolder::Rotate },
	ready_logic{ ready_logic }
{
	SetPosition(position);
}

void MenuHolder::InitBaseCatchers()
{
	ButtonCatchUp(SYSTEM_BTN_UP);
	ButtonCatchDown(SYSTEM_BTN_DOWN);
	ButtonCatchEnter(SYSTEM_BTN_ENTER);
	EncoderCatch(SYSTEM_ENC_MAIN);
}

void MenuHolder::EnableReadyLogic()
{
	ready_logic = true;
}

void MenuHolder::ButtonCatchUp(uint8_t button_id)
{
	up_catcher.Catch(button_id, BCM_SINGLE_PRESS | BCM_MULTI_HOLD);
	up_catcher.SetHoldTime(200, 100);
}

void MenuHolder::ButtonCatchDown(uint8_t button_id)
{
	down_catcher.Catch(button_id, BCM_SINGLE_PRESS | BCM_MULTI_HOLD);
	down_catcher.SetHoldTime(200, 100);
}

void MenuHolder::ButtonCatchEnter(uint8_t button_id)
{
	enter_cather.Catch(button_id, BCM_SINGLE_PRESS);
}

void MenuHolder::EncoderCatch(uint8_t encoder_id)
{
	enc_catcher.Catch(encoder_id, ECM_ROTATE);
}

void MenuHolder::OnShow()
{
	if (reset_on_show) 
		ResetCurrentID();
}

void MenuHolder::OnHide()
{
	enter_cather.Reset();
	up_catcher.Reset();
	down_catcher.Reset();
	enc_catcher.Reset();
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

void MenuHolder::Rotate(bool left)
{
	if (left)	Up();
	else		Down();
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
			elem_offset_x -= (TextWidth(GetText(i), font) / 2);
		}

		DrawText(
			screen,
			{style == MenuStyle::STYLE_3 ? elem_offset_x : CharWidth + elem_offset_x, elem_offset_y},
			GetText(i),
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
		ready_logic && enter_cather.ProcessInput(event) ||
		enc_catcher.ProcessInput(event))
		return true;

	return false;
}

void MenuHolder::Loop()
{
	up_catcher.Loop();
	down_catcher.Loop();
	enter_cather.Loop();
	enc_catcher.Loop();
}
