#include <UI/ICursor.hpp>

ICursor::ICursor(int min_row, int max_row, int min_col, int max_col)
	: min_row{ min_row }, max_row{ max_row }, min_col{ min_col }, max_col{ max_col }
{
	cur_row = min_row;
	cur_col = min_col;
}

void ICursor::SetCell(int cur_row, int cur_col)
{
	this->cur_row = cur_row;
	this->cur_col = cur_col;
}

void ICursor::GetCell(int& cur_row, int& cur_col) const
{
	cur_row = this->cur_row;
	cur_col = this->cur_col;
}

void ICursor::Up()
{
	if (cur_row > min_row)
		cur_row--;
}

void ICursor::Down()
{
	if (cur_row < max_row - 1)
		cur_row++;
}

void ICursor::Left()
{
	if (cur_col > min_col)
		cur_col--;
}

void ICursor::Right()
{
	if (cur_col < max_col - 1)
		cur_col++;
}

void ICursor::ScrollLeft()
{
	if (cur_col > min_col)
		Left();
	else if (cur_row > min_row)
	{
		Up();
		cur_col = max_col - 1;
	}
}

void ICursor::ScrollRight()
{
	if (cur_col < max_col - 1)
		Right();
	else if (cur_row < max_row - 1)
	{
		Down();
		cur_col = min_col;
	}
}

void ICursor::Draw(IScreen& screen, Point2_i offset)
{
	if (!IsActive())
		return;
	DrawCursor(screen, offset);
}
