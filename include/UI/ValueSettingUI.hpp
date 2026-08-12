/**
 * @file ValueSettingUI.hpp
 * @date Dec 20, 2025
 * @author Rakhimov T.
 */

#ifndef INC_UI_VALUE_SETTING_UI_HPP_
#define INC_UI_VALUE_SETTING_UI_HPP_

#include <UI/ISettingUI.hpp>
#include <Data/IDataCell.hpp>
#include <Data/StaticText.hpp>

#include <string.h>
#include <stdio.h>


// UI элемент для отрисовки значения IDataCell
template <typename T>
class ValueSettingUI : public virtual ISettingUI
{
protected:
	const StaticText32 N_A = "N/A";

	IDataCell<T>* cell{};
	T* value_ptr{};

	T current_value{};
	bool valid_value{};

	StaticText32 fmt;

	// Текущее значение настройки
	StaticText32 CurrentValueRepr() override
	{
		if (valid_value)
		{
			char value_txt[64]{};
			snprintf(value_txt, sizeof(value_txt), fmt, current_value);
			return value_txt;
		}
		return N_A;
	}

public:
	ValueSettingUI(const StaticText32& name, Point2_i position, IDataCell<T>* cell, StaticText32 fmt)
		: ISettingUI{ name, position }, cell{ cell }, value_ptr{ nullptr }, fmt{ fmt }
	{
	}

	ValueSettingUI(const StaticText32& name, Point2_i position, T* value_ptr, StaticText32 fmt)
		: ISettingUI{ name, position }, cell{ nullptr }, value_ptr{ value_ptr }, fmt{ fmt }
	{
	}

	/* Обновляет текущее значение */
	virtual void UpdateCurrentValue() override
	{
		if (cell)
		{
			valid_value = cell->Get(current_value);
			return;
		}
		
		if (value_ptr)
		{
			valid_value = true;
			current_value = *value_ptr;
			return;
		}
	}
};

#include <Input/SystemInputID.hpp>

template <typename T>
class ValueEditableSettingUI : public ValueSettingUI<T>, public IEditableSettingUI
{
protected:
	T edit_value{};
	T min{}, max{}, step{};

	bool bounded = true;

	StaticText32 EditingValueRepr() override
	{
		char value_txt[64]{};
		snprintf(value_txt, sizeof(value_txt), ValueSettingUI<T>::fmt, edit_value);
		return value_txt;
	}

	void SetEditingValue() override
	{	
		if (ValueSettingUI<T>::cell)
		{
			ValueSettingUI<T>::cell->Set(edit_value);
			return;
		}

		if (ValueSettingUI<T>::value_ptr)
		{
			*ValueSettingUI<T>::value_ptr = edit_value;
			return;
		}
	}

	bool ProcessEditingEvent(const AppEvent& event) override
	{
		if (IsSystemPrevEvent(event, true))
		{
			if (edit_value > min)
				edit_value -= step;
			else if (!bounded)
				edit_value = max;
			return true;
		}
		else if (IsSystemNextEvent(event, true)) 
		{
			if (edit_value < max)
				edit_value += step;
			else if (!bounded)
				edit_value = min;
			return true;
		}
		return false;
	}

public:
	ValueEditableSettingUI(const StaticText32& name, Point2_i position, IDataCell<T>* cell, StaticText32 fmt, T min, T max, T step, bool bounded = true)
		: ISettingUI{ name, position },
		ValueSettingUI<T>{ name, position, cell, fmt },
		IEditableSettingUI{ name, position },
		min{ min }, max{ max }, step{ step },
		bounded{ bounded }
	{
	}

	ValueEditableSettingUI(const StaticText32& name, Point2_i position, T* value_ptr, StaticText32 fmt, T min, T max, T step, bool bounded = true)
		: ISettingUI{ name, position },
		ValueSettingUI<T>{ name, position, value_ptr, fmt },
		IEditableSettingUI{ name, position },
		min{ min }, max{ max }, step{ step },
		bounded{ bounded }
	{
	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
		if (IsCaptured()) return;

		ValueSettingUI<T>::UpdateCurrentValue();
		edit_value = ValueSettingUI<T>::current_value;
		if (!ValueSettingUI<T>::valid_value)
			edit_value = min;
	}

};

#endif
