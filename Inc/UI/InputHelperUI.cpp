#include <UI/InputHelperUI.hpp>

#include <Graphics/BitmapData.hpp>
#include <DrawFunctions/DrawBitmap.hpp>

#include <algorithm>

static uint8_t __empty__[] = {
  0b00000110,
  0b00001001,
  0b00001001,
  0b00000110,
};
static uint8_t __fill__[] = {
  0b00000110,
  0b00001111,
  0b00001111,
  0b00000110,
};
BitmapData BM_EmptyButton{ 4, 4, __empty__ };
BitmapData BM_FillButton{ 4, 4, __fill__ };


bool InputHelperUI::Setted(HelperButton button)
{
	return std::find(buttons.begin(), buttons.end(), button) != buttons.end();
}

InputHelperUI::InputHelperUI(Point2_i position, int period)
{
	SetPosition(position);
	ResetTimer(period);
}

void InputHelperUI::ResetHelper()
{
	buttons.clear();
	turn = HelperTurn::NONE;
}

void InputHelperUI::ResetTimer(int period)
{
	if (period != 0)
		timer.Start(period);
	else
		timer.Start();
}

void InputHelperUI::SetButton(HelperButton button)
{
	buttons.push_back(button);
}

void InputHelperUI::SetTurn(HelperTurn turn)
{
	this->turn = turn;
}

void InputHelperUI::Draw(IScreen& screen, Point2_i offset)
{
	bool fill = false;
	if (!timer.Expired())
	{
		if (timer.GetProgress() < 0.5f)
			fill = true;
	}
	else
	{
		ResetTimer();
	}

	using namespace DrawFunctions;
	// Left
	DrawBitmap(screen, position + offset + Point2_i{0, 3}, fill && Setted(HelperButton::LEFT) ? BM_FillButton : BM_EmptyButton, WhiteColor);
	// Right
	DrawBitmap(screen, position + offset + Point2_i{8, 3}, fill && Setted(HelperButton::RIGHT) ? BM_FillButton : BM_EmptyButton, WhiteColor);
	// Up
	DrawBitmap(screen, position + offset + Point2_i{4, 0}, fill && Setted(HelperButton::UP) ? BM_FillButton : BM_EmptyButton, WhiteColor);
	// Down
	DrawBitmap(screen, position + offset + Point2_i{4, 6}, fill && Setted(HelperButton::DOWN) ? BM_FillButton : BM_EmptyButton, WhiteColor);
}
