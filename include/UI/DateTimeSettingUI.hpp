/**
 * @file DateTimeSettingUI.hpp
 * @date Jul 11, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_DATE_TIME_SETTING_UI_HPP_
#define INC_UI_DATE_TIME_SETTING_UI_HPP_

#include <UI/ISettingUI.hpp>
#include <Data/IDataCell.hpp>
#include <Data/StaticText.hpp>
#include <Time/DateTime.hpp>

#include <string.h>
#include <stdio.h>


class DateTimeSettingUI : public virtual ISettingUI
{
public:
    enum DateTimeShow { TIME, DATE, DATE_TIME };

protected:
    DateTimeShow type;

    struct DateTime_Data
    {
        uint8_t hours, minutes, seconds;
        uint8_t date, month, year;
    }
    current;

    void Make(char* buffer, uint32_t size, DateTimeShow type, DateTime_Data& data)
    {
        switch (type)
        {
        case TIME:
            snprintf(buffer, size, "%02d:%02d:%02d", data.hours, data.minutes, data.seconds);
            break;
        case DATE:
            snprintf(buffer, size, "%02d.%02d.%02d", data.date, data.month, data.year);
            break;
        }
    }

	// Текущее значение настройки
	StaticText32 CurrentValueRepr() override
	{
        char value_txt[64]{};

        switch (type)
        {
        case TIME:
        case DATE:
            Make(value_txt, sizeof(value_txt), type, current);
            break;
        case DATE_TIME:
            Make(value_txt, sizeof(value_txt), TIME, current);
            value_txt[8] = ' ';
            Make(&value_txt[9], sizeof(value_txt) - 8, DATE, current);
            break;
        default:
            break;
        }

        return value_txt;
    }

public:
	DateTimeSettingUI(const StaticText32& name, Point2_i position, DateTimeShow type)
		: ISettingUI{ name, position }, type{ type }
	{
	}

	/* Обновляет текущее значение */
	virtual void UpdateCurrentValue() override
	{
        DateTime::GetTime(current.hours,    current.minutes,    current.seconds);
        DateTime::GetDate(current.date,     current.month,      current.year);
	}
};

#include <DrawFunctions/DrawText.hpp>
#include <Input/SystemInputID.hpp>
#include <Data/MinMaxChanger.hpp>

class DateTimeEditableSettingUI : public DateTimeSettingUI, public IEditableSettingUI
{
protected:
    int CharWidth = 8, CharHeight = 8;

	DateTime_Data edit_data{};

	int edit_param = 0;

	StaticText32 EditingValueRepr() override
	{
        char value_txt[64]{};

        switch (type)
        {
        case TIME:
        case DATE:
            Make(value_txt, sizeof(value_txt), type, edit_data);
            break;
        default:
            break;
        }

        return value_txt;
	}

	void SetEditingValue() override
	{
        switch (type)
        {
        case TIME:
            DateTime::SetTime(edit_data.hours, edit_data.minutes, edit_data.seconds);
            break;
        case DATE:
            DateTime::SetDate(edit_data.date, edit_data.month, edit_data.year);
            break;
        }
	}

    void ChangeParam(int param, uint8_t value, bool plus)
    {
        switch (type)
        {
        case TIME:
            switch (param)
            {
            case 0:
                MinMaxChanger<uint8_t>(&edit_data.hours, 0, 23, true).Change(value, plus);
                break;
            case 1:
                MinMaxChanger<uint8_t>(&edit_data.minutes, 0, 59, true).Change(value, plus);
                break;
            case 2:
                MinMaxChanger<uint8_t>(&edit_data.seconds, 0, 59, true).Change(value, plus);
                break;
            default:
                break;
            }
            break;
        case DATE:
            switch (param)
            {
            case 0:
                MinMaxChanger<uint8_t>(&edit_data.date, 1, 31, true).Change(value, plus);
                break;
            case 1:
                MinMaxChanger<uint8_t>(&edit_data.month, 1, 12).Change(value, plus);
                break;
            case 2:
                MinMaxChanger<uint8_t>(&edit_data.year, 0, 99, true).Change(value, plus);
                break;
            default:
                break;
            }
            break;
        }
    }

	bool ProcessEditingEvent(const AppEvent& event) override
	{
		// TODO: определить алфавит
		const char MIN = 48, MAX = 90;

		if (IsSystemLeftEvent(event, true))
		{
			if (edit_param > 0)
				edit_param--;
			return true;
		}
		else if (IsSystemRightEvent(event, true))
		{
			if (edit_param < 2)
				edit_param++;
			return true;
		}
		else if (IsSystemPrevEvent(event, true))
		{
            ChangeParam(edit_param, 1, false);
			return true;
		}
		else if (IsSystemNextEvent(event, true))
		{
            ChangeParam(edit_param, 1, true);
			return true;
		}

		return false;
	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
        if (IsCaptured()) return;

        DateTimeSettingUI::UpdateCurrentValue();
		edit_data = current;
		if (setting_font)
		{
			CharWidth = setting_font->GetWidth();
			CharHeight = setting_font->GetHeight();
		}
	}

	virtual void DrawValue(IScreen& screen) override
	{
		using namespace DrawFunctions;

		if (IsCaptured())
		{
			// TODO: добавить blink, основанный на таймере
			// Инверсия заливки при редактировании
			// Прописать сюда заливку сначала белым вокруг, потом черным - поля, которое редактируется

            StaticText32 edit_text = EditingValueRepr();
			DrawText(screen, position + Point2_i{name_value_offset, 0}, edit_text, WhiteColor, BlackColor, false, setting_font);

            if (edit_param > 3) return;
            int edit_char = edit_param * 3;

            for (int i = 0; i < 2; ++i)
            {
                DrawChar(screen, position + Point2_i{name_value_offset + (edit_char + i) * CharWidth, 0}, &edit_text.ConstChar()[edit_char + i], WhiteColor, BlackColor, true, setting_font);
            }

		}
		else
		{
			edit_param = 0;
			DrawText(screen, position + Point2_i{name_value_offset, 0}, CurrentValueRepr(), WhiteColor, BlackColor, false, setting_font);
		}
	}

public:
	DateTimeEditableSettingUI(const StaticText32& name, Point2_i position, DateTimeShow type)
		: ISettingUI{ name, position }, 
        DateTimeSettingUI{name, position, type}, 
        IEditableSettingUI{ name, position }
	{

	}
};

#endif
