/*
 * BitmapSettingUI.hpp
 *
 *  Created on: Dec 17, 2025
 *      Author: Timur
 */

#ifndef INC_UI_BITMAP_SETTING_UI_HPP_
#define INC_UI_BITMAP_SETTING_UI_HPP_

#include <UI/ISettingUI.hpp>

#include <Data/StaticText.hpp>
#include <Data/StaticListView.hpp>

#include <Data/IDataCell.hpp>

#include <Input/SystemInputID.hpp>

#include <Graphics/BitmapData.hpp>
#include <DrawFunctions/DrawLine.hpp>
#include <DrawFunctions/DrawBitmap.hpp>

// Настройка с выбором одного из вариантов
template <typename T>
class BitmapSettingUI : public virtual ISettingUI
{
protected:
	const StaticListView<BitmapData> list;

	// Ячейка с ID битмапа
	IDataCell<T>* ID_cell;

	// Индекс битмапа в списке
	int cur_idx = -1;

	StaticText32 CurrentValueRepr() override
	{
		return "BM";
	}

	void DrawBitmap(IScreen& screen, const BitmapData& bitmap, bool up_arrow = false, bool down_arrow = false)
	{
		using namespace DrawFunctions;

		int x = position.x + name_value_offset;
		int y = position.y + (8 - bitmap.height) / 2;
		DrawFunctions::DrawBitmap(screen, {x, y}, bitmap, WhiteColor);

		x += bitmap.width / 2;
		y += bitmap.height / 2;

		if (up_arrow)
		{
			int arrow_y = y - bitmap.height / 2 - 5;
			Point2_i up{ x, arrow_y };
			DrawLine(screen, up, { x - 5, up.y + 2 }, WhiteColor);
			DrawLine(screen, up, { x + 5, up.y + 2 }, WhiteColor);
		}

		if (down_arrow)
		{
			int arrow_y = y + bitmap.height / 2 + 4;
			// TODO: проверить минус {5, 2}
			Point2_i up{ x, arrow_y };
			DrawLine(screen, up, { x - 5, up.y - 2 }, WhiteColor);
			DrawLine(screen, up, { x + 5, up.y - 2 }, WhiteColor);
		}
	}

public:
	BitmapSettingUI(const StaticText32& name, Point2_i position, IDataCell<T>* ID_cell, StaticListView<BitmapData> list)
		: ISettingUI{ name, position }, ID_cell{ ID_cell }, list{ list }
	{
	}

	void DrawValue(IScreen& screen) override
	{
		if (cur_idx < 0 || cur_idx >= list.Count())
			return;

		DrawBitmap(screen, list[cur_idx]);
	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
		cur_idx = -1;
		T get_ID = ID_cell->GetOrDefault();
		for (int idx = 0; idx < list.Count(); ++idx)
		{
			if (list[idx].ID == get_ID)
			{
				cur_idx = idx;
				break;
			}
		}
	}
};

// Настройка с выбором одного из вариантов
template <typename T>
class BitmapEditableSettingUI : public BitmapSettingUI<T>, public IEditableSettingUI
{
protected:
	// Редактируемый индекс битмапа в списке
	int edit_idx = -1;

	bool bounded = true;

	StaticText32 EditingValueRepr() override
	{
		return "BM";
	}

	void SetEditingValue() override
	{
		if (edit_idx == -1 || edit_idx == BitmapSettingUI<T>::cur_idx)
			return;

		BitmapSettingUI<T>::ID_cell->Set(BitmapSettingUI<T>::list[edit_idx].ID);
	}

	bool ProcessEditingEvent(const AppEvent& event) override
	{
		if (IsSystemPrevEvent(event, true))
		{
			if (edit_idx > 0)
				edit_idx--;
			else if (!bounded)
				edit_idx = (int)BitmapSettingUI<T>::list.Count() - 1;
			return true;
		}
		else if (IsSystemNextEvent(event, true))
		{
			if (edit_idx < (int)BitmapSettingUI<T>::list.Count() - 1)
				edit_idx++;
			else if (!bounded)
				edit_idx = 0;
			return true;
		}
		return false;
	}

public:
	BitmapEditableSettingUI(const StaticText32& name, Point2_i position, IDataCell<T>* ID_cell, StaticListView<BitmapData> list, bool bounded = true)
		: ISettingUI{ name, position },
		BitmapSettingUI<T>{ name, position, ID_cell, list },
		IEditableSettingUI{ name, position },
		bounded{ bounded }
	{
	}

	void DrawValue(IScreen& screen) override
	{
		if (IsCaptured())
		{
			if (edit_idx < 0 || edit_idx >= BitmapSettingUI<T>::list.Count())
			{
				Capture(false);
				return;
			}

			BitmapSettingUI<T>::DrawBitmap(screen, BitmapSettingUI<T>::list[edit_idx], true, true);

			return;
		}

		BitmapSettingUI<T>::DrawValue(screen);
	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
		BitmapSettingUI<T>::UpdateCurrentValue();
		edit_idx = BitmapSettingUI<T>::cur_idx;
		if (edit_idx < 0)
			edit_idx = 0;
	}

};

#endif
