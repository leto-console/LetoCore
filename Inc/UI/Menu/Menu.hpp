/*
 * Menu.hpp
 *
 *  Created on: Jan 13, 2026
 *      Author: Timur
 */

#ifndef INC_UI_MENU_HPP_
#define INC_UI_MENU_HPP_

#include "MenuHolder.hpp"
#include <Data/StaticText.hpp>
#include <Data/StaticList.hpp>

template <uint8_t MaxSize>
class Menu : public MenuHolder
{
protected:
	StaticList<StaticText32, MaxSize> texts;

public:
	Menu(uint8_t visible_elements = 8, Point2_i position = {}) : MenuHolder{ texts, visible_elements, position }
	{
	}

	void SetText(uint8_t ID, const StaticText32& text)
	{
		if (ID >= Count()) return;
		texts[ID] = text;
	}

	void AppendMenuItem(const StaticText32& text)
	{
		texts.Push(text);
	}

	virtual void Clear()
	{
		texts.Clear();
	}
};

#endif
