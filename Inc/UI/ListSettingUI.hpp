/**
 * @file ListSettingUI.hpp
 * @date Dec 17, 2025
 * @author Rakhimov T.
 */

#ifndef INC_UI_LIST_SETTING_UI_HPP_
#define INC_UI_LIST_SETTING_UI_HPP_

#include <UI/ISettingUI.hpp>

#include <Data/StaticListView.hpp>

#include <Data/IDataCell.hpp>

#include <Input/SystemInputID.hpp>

template <typename T>
struct ListSettingItem
{
	StaticText32 name;
	T value;
};

 // Настройка с выбором одного из вариантов
template <typename T>
class ListSettingUI : public virtual ISettingUI
{
protected:
	const StaticText32 N_A = "N/A";

	IDataCell<T>* cell;
	const StaticListView<ListSettingItem<T>> list;

	int cur_idx = -1;

	StaticText32 CurrentValueRepr() override
	{
		if (cur_idx == -1)
			return N_A;
		return list[cur_idx].name;
	}

public:
	ListSettingUI(const StaticText32& name, Point2_i position, IDataCell<T>* cell, StaticListView<ListSettingItem<T>> list)
		: ISettingUI{ name, position }, cell{ cell }, list{ list }
	{

	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
		if (IsCaptured()) return;

		cur_idx = -1;
		T get_value = cell->GetOrDefault();
		for (int i = 0; i < (int)list.Count(); i++)
		{
			if (list[i].value == get_value)
			{
				cur_idx = i;
				break;
			}
		}
	}
};

// Настройка с выбором одного из вариантов
template <typename T>
class ListEditableSettingUI : public ListSettingUI<T>, public IEditableSettingUI
{
protected:
	const StaticText32 N_A = "N/A";

	int edit_idx = -1;

	bool bounded = true;

	StaticText32 EditingValueRepr() override
	{
		if (edit_idx == -1)
			return N_A;
		return ListSettingUI<T>::list[edit_idx].name;
	}

	void SetEditingValue() override
	{
		if (edit_idx == -1 || edit_idx == ListSettingUI<T>::cur_idx)
			return;
		ListSettingUI<T>::cell->Set(ListSettingUI<T>::list[edit_idx].value);
	}

	bool ProcessEditingEvent(const AppEvent& event) override
	{
		if (IsSystemPrevEvent(event, true))
		{
			if (edit_idx > 0)
				edit_idx--;
			else if (!bounded)
				edit_idx = (int)ListSettingUI<T>::list.Count() - 1;
			return true;
		}
		else if (IsSystemNextEvent(event, true))
		{
			if (edit_idx < (int)ListSettingUI<T>::list.Count() - 1)
				edit_idx++;
			else if (!bounded)
				edit_idx = 0;
			return true;
		}
		return false;
	}

public:
	ListEditableSettingUI(const StaticText32& name, Point2_i position, IDataCell<T>* cell, StaticListView<ListSettingItem<T>> list, bool bounded = true)
		: ISettingUI{ name, position },
		ListSettingUI<T>{ name, position, cell, list },
		IEditableSettingUI{ name, position },
		bounded{ bounded }
	{
	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
		if (IsCaptured()) return;

		ListSettingUI<T>::UpdateCurrentValue();
		edit_idx = ListSettingUI<T>::cur_idx;
	}

};

#endif
