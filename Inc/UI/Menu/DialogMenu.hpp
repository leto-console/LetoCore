/*
 * DialogMenu.hpp
 *
 *  Created on: Feb25, 2026
 *      Author: Timur
 */

#ifndef INC_UI_DIALOG_MENU_HPP_
#define INC_UI_DIALOG_MENU_HPP_

#include <LetoAPI_V1/LetoAPI_V1.hpp>

#include <Drawable/IDrawable.hpp>
#include <SceneManager/ISceneObject.hpp>
#include <Input/SystemInputID.hpp>
#include <Input/InputCatcher.hpp>
#include <Input/ButtonEvent.hpp>
#include <DrawFunctions/DrawLine.hpp>
#include <DrawFunctions/DrawRectangle.hpp>
#include <DrawFunctions/DrawText.hpp>

#include <UI/Menu/ParamMenu.hpp>

template <typename Param, uint8_t MaxSize>
class DialogParamMenu : public ISceneObject
{
protected:
	enum { TEXT_SIZE = 3 };

	StaticText32 text[TEXT_SIZE]{};
	ParamMenu<Param, MaxSize> menu;
	bool ready = false;

	uint32_t active_ms{};

	InputCatcher<DialogParamMenu> enter_catcher;

	IFont* font;

public:
	DialogParamMenu() : menu{ 3 }, enter_catcher{ &ButtonEvent::Compare }
	{
		font = leto_api_v1->Font->GetFont(7, 7, 0);
		menu.SetStyle(MenuStyle::STYLE_3, leto_api_v1->Font->GetFont(7, 7, 1));
		menu.SetHorizonAlignment(MenuHorizonAlignment::CENTER);
		Disable();
	}

	void InitBaseCatchers()
	{
		RegUpEvent(&IsSystemPrevEvent);
		RegDownEvent(&IsSystemNextEvent);
		RegEnterEvent(&IsSystemEnterEvent);
	}

	void OnShow() override
	{
		ResetCurrentID();
	}

	void RegUpEvent(IsEventFunc is_event) 		{ menu.RegUpEvent(is_event); }
	void RegDownEvent(IsEventFunc is_event) 	{ menu.RegDownEvent(is_event); }
	void RegEnterEvent(IsEventFunc is_event) 	{ enter_catcher.Catch(is_event); }

	void SetText(int level, const StaticText32& text)
	{
		if (level >= TEXT_SIZE)
			return;
		this->text[level] = text;
	}

	void AppendMenuItem(const StaticText32& text, Param param)
	{
		menu.AppendMenuItem(text, param);
	}

	void Clear()
	{
		menu.Clear();
	}

	void ResetCurrentID()
	{
		menu.ResetCurrentID();
	}

	// Флаг готовности результата
	bool IsResultReady(Param& param) const
	{
		if (!IDrawable::IsActive() || !ready)
			return false;
		param = menu.GetCurrentParam();
		return true;
	}

	// Включить отображение
	void Enable()
	{
		ISceneObject::Enable();
		ready = false;
		menu.Capture(true);
		menu.ResetCurrentID();
		active_ms = leto_api_v1->Globals->GetCurrentMs();
	}

	// Снять с отображения
	void Disable()
	{
		ISceneObject::Disable();
		ready = false;
		menu.Capture(false);
	}

	void Draw(IScreen& screen, Point2_i offset = {}) override
	{
		using namespace DrawFunctions;

		if (!IsActive())
			return;
		
		Point2_i pos = position + offset;

		int left{ 64 }, right{ };
		int up{ 4 }, down{ 59 };
		for (int i = 0; i < TEXT_SIZE; ++i)
		{
			int w_2 = TextWidth(text[i], font) / 2;
			if (pos.x - w_2 < left)
				left = pos.x - w_2;
			if (pos.x + w_2 > right)
				right = pos.x + w_2;
		}
		for (int i = 0; i < menu.Count(); ++i)
		{
			int w_2 = TextWidth(menu.GetText(i)) / 2;
			if (pos.x - w_2 < left)
				left = pos.x - w_2;
			if (pos.x + w_2 > right)
				right = pos.x + w_2;
		}

		left -= 4;
		right += 4;

		// Меню с рамками
		DrawOutlinedRectangle(screen, { left, up }, { right, down }, BlackColor, WhiteColor);

		// TODO: offset's
		for (int i = 0; i < TEXT_SIZE; ++i)
			DrawText(
				screen,
				{pos.x - TextWidth(text[i], font) / 2, 8 * (i + 1)},
				text[i],
				WhiteColor, BlackColor,
				false, font);

		menu.Draw(screen, {pos.x, 36});
	}

	bool ProcessInput(const AppEvent& input) override
	{
		if (menu.ProcessInput(input))
			return true;
		else if ((enter_catcher.ProcessInput(input) || IsSystemEnterEvent(input)) && leto_api_v1->Globals->GetCurrentMs() - active_ms > 200)
			ready = true;

		return true;
	}
};

#endif