/**
 * @file TextSettingUI.hpp
 * @date Dec 17, 2025
 * @author Rakhimov T.
 */

#ifndef INC_UI_TEXT_SETTING_UI_HPP_
#define INC_UI_TEXT_SETTING_UI_HPP_

#include <UI/ISettingUI.hpp>
#include <Data/IDataCell.hpp>
#include <Data/StaticText.hpp>
#include <Input/SystemInputID.hpp>
#include <DrawFunctions/DrawLine.hpp>
#include <DrawFunctions/DrawText.hpp>

 // Настройка текста UI
template <int N = 8>
class TextEditableSettingUI : public IEditableSettingUI
{
protected:
	const StaticText32 N_A = "N/A";

	int CharWidth = 8, CharHeight = 8;

	IDataCell<StaticText<N>>* setting;

	StaticText<N> cur_text{};
	StaticText<N> edit_text{};

	int edit_char = 0;

	// Нужна ли обводка вокруг текста
	bool outline{};

	StaticText32 CurrentValueRepr() override
	{
		return cur_text;
	}

	StaticText32 EditingValueRepr() override
	{
		return edit_text;
	}

	void SetEditingValue() override
	{
		setting->Set(edit_text);
	}

	bool ProcessEditingEvent(const AppEvent& event) override
	{
		// TODO: определить алфавит
		const char MIN = 48, MAX = 90;

		if (IsSystemLeftEvent(event, true))
		{
			if (edit_char > 0)
				edit_char--;
			return true;
		}
		else if (IsSystemRightEvent(event, true))
		{
			if (edit_char < N - 1)
				edit_char++;
			return true;
		}
		else if (IsSystemPrevEvent(event, true))
		{
			if (edit_text[edit_char] <= MIN)
				edit_text[edit_char] = MIN;
			else if (edit_text[edit_char] <= MAX)
				edit_text[edit_char]--;
			else if (edit_text[edit_char] > MAX)
				edit_text[edit_char] = MAX;
			return true;
		}
		else if (IsSystemNextEvent(event, true))
		{
			if (edit_text[edit_char] < MIN)
				edit_text[edit_char] = MIN;
			else if (edit_text[edit_char] < MAX)
				edit_text[edit_char]++;
			else if(edit_text[edit_char] >= MAX)
				edit_text[edit_char] = MAX;
			return true;
		}

		return false;
	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
		if (IsCaptured()) return;

		cur_text = edit_text = setting->GetOrDefault();
		if (setting_font)
		{
			CharWidth = setting_font->GetWidth();
			CharHeight = setting_font->GetHeight();
		}
	}

	virtual void DrawValue(IScreen& screen) override
	{
		using namespace DrawFunctions;

		if (outline)
		{
			Point2_i left_top{ position.x + name_value_offset - 2, position.y - 2 };
			Point2_i right_bottom{ position.x + name_value_offset + N * CharWidth + 1, position.y + CharHeight + 1 };
			Point2_i left_bottom{ left_top.x, right_bottom.y };
			Point2_i right_top{ right_bottom.x, left_top.y };

			DrawLine(screen, left_top, left_bottom, WhiteColor);
			DrawLine(screen, left_top, right_top, WhiteColor);
			DrawLine(screen, right_bottom, right_top, WhiteColor);
			DrawLine(screen, right_bottom, left_bottom, WhiteColor);
		}

		if (IsCaptured())
		{
			// TODO: добавить blink, основанный на таймере
			// Инверсия заливки при редактировании
			// Прописать сюда заливку сначала белым вокруг, потом черным - поля, которое редактируется

			DrawText(screen, position + Point2_i{name_value_offset, 0}, EditingValueRepr(), WhiteColor, BlackColor, false, setting_font);
			DrawChar(screen, position + Point2_i{name_value_offset + edit_char * CharWidth, 0}, &edit_text.ConstChar()[edit_char], WhiteColor, BlackColor, true, setting_font);
		}
		else
		{
			edit_char = 0;
			DrawText(screen, position + Point2_i{name_value_offset, 0}, CurrentValueRepr(), WhiteColor, BlackColor, false, setting_font);
		}
	}

public:
	TextEditableSettingUI(const StaticText32& name, Point2_i position, IDataCell<StaticText<N>>* setting, bool outline = false)
		: ISettingUI{ name, position }, IEditableSettingUI{ name, position }, setting{ setting }, outline{ outline }
	{
	}
};

#endif
