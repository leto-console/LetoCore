/*
 * ICursor.hpp
 *
 *  Created on: Dec 13, 2025
 *      Author: Timur
 */

#ifndef INC_UI_I_CURSOR_HPP_
#define INC_UI_I_CURSOR_HPP_

#include "LibrariesExport.h"

#include <Drawable/IDrawable.hpp>

#include <stdint.h>

class LIBRARIES_EXPORT ICursor : public IDrawable
{
protected:
	int min_row{}, max_row{};
	int min_col{}, max_col{};

	int cur_row{}, cur_col{};

	// Отрисовать курсор в текущем положении
	virtual void DrawCursor(IScreen& screen, Point2_i offset = {}) = 0;

public:
	ICursor(int min_row, int max_row, int min_col, int max_col);

	void SetCell(int cur_row, int cur_col);
	void GetCell(int& cur_row, int& cur_col) const;

	void Up();
	void Down();
	void Left();
	void Right();

	void ScrollLeft();
	void ScrollRight();

	void Draw(IScreen& screen, Point2_i offset = {}) override;
};

#endif
