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
	Menu(uint8_t visible_elements = 8, Point2_i position = {}, bool ready_logic = false) : MenuHolder{ visible_elements, position, ready_logic }
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

	uint8_t Count() const override 
	{
		return texts.Count();
	}

	const StaticText32& GetText(uint8_t ID) const override
	{
		if (ID >= texts.Count())
			return texts.Back();
		return texts[ID];
	}

};

#endif
