/*
 * ParamMenu.hpp
 *
 *  Created on: Jan 13, 2026
 *      Author: Timur
 */

#ifndef INC_UI_PARAM_MENU_HPP_
#define INC_UI_PARAM_MENU_HPP_

#include <UI/Menu/Menu.hpp>
#include <Data/StaticText.hpp>
#include <Data/StaticList.hpp>

template <typename Param, uint8_t MaxSize>
class ParamMenu : public Menu<MaxSize>
{
protected:
	StaticList<Param, MaxSize> params;

public:
	ParamMenu(uint8_t height = 8, Point2_i position = {}, bool ready_logic = false)
		: Menu<MaxSize>{ height, position, ready_logic }
	{
	}

	void AppendMenuItem(const StaticText32& text, Param param = {})
	{
		Menu<MaxSize>::AppendMenuItem(text);
		params.Push(param);
	}

	Param GetCurrentParam() const { return params[MenuHolder::currentID]; }

	Param GetParam(uint8_t ID) const 
	{
		if (ID >= params.Count())
			return params.Back();
		return params[ID];
	}

	void Clear() override
	{
		params.Clear();
		Menu<MaxSize>::Clear();
	}
};

#endif
