/*
 * InputHelperUI.hpp
 *
 *  Created on: Jan 13, 2026
 *      Author: Timur
 */

#ifndef INC_UI_INPUT_HELPER_UI_HPP_
#define INC_UI_INPUT_HELPER_UI_HPP_

#include "LibrariesExport.h"

#include <Drawable/IDrawable.hpp>
#include <Time/Timer.hpp>

#include <Data/StaticList.hpp>

// Графический элемент-помощник ввода
class LIBRARIES_EXPORT InputHelperUI : public IDrawable
{
public:
	enum class HelperButton : char
	{
		LEFT, RIGHT, UP, DOWN, ENTER
	};
	enum class HelperTurn : char
	{
		NONE, LEFT, RIGHT, BOTH
	};

protected:
	StaticList<HelperButton, 16> buttons;
	HelperTurn turn = HelperTurn::NONE;

	Timer timer;

	bool Setted(HelperButton button);

public:
	InputHelperUI(Point2_i position = {}, int period = 1000);

	void ResetHelper();
	void ResetTimer(int period = 0);
	
	void SetButton(HelperButton button);
	void SetTurn(HelperTurn turn);

	void Draw(IScreen& screen, Point2_i offset = {}) override;
};

#endif
