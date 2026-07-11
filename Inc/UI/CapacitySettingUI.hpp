/*
 * CapacitySettingUI.hpp
 *
 *  Created on: Mar 08, 2026
 *      Author: Timur
 */

#ifndef INC_UI_CAPACITY_SETTING_UI_HPP_
#define INC_UI_CAPACITY_SETTING_UI_HPP_

#include <UI/ISettingUI.hpp>
#include <Data/ICapacity.hpp>

enum class CapacitySettingsStyle : uint8_t
{
	STYLE_1,	// Стиль "46.32%"
	STYLE_2,	// Стиль "3.23/16.00 Кб"
	STYLE_3,	// Стиль "3.23/16.00 Кб (46.32%)"
};

// Настройка с выбором одного из вариантов
class CapacitySettingUI : public virtual ISettingUI
{
protected:
	const StaticText32 N_A = "N/A";

	ICapacity* object;
	CapacitySettingsStyle style;

	StaticText32 CurrentValueRepr() override
	{
		StaticText32 repr;

		if (style == CapacitySettingsStyle::STYLE_1)
			snprintf(repr.CharPtr(), repr.Capacity(), "%.2f%%", object->GetPercentage());
		else if (style == CapacitySettingsStyle::STYLE_2)
			snprintf(repr.CharPtr(), repr.Capacity(), "%.2f/%.2f Кб", 
				object->GetUsedBytes() / 1024.f, object->GetTotalBytes() / 1024.f);
		else if (style == CapacitySettingsStyle::STYLE_3)
			snprintf(repr.CharPtr(), repr.Capacity(), "%.2f/%.2f Кб (%.2f%%)", 
				object->GetUsedBytes() / 1024.f, object->GetTotalBytes() / 1024.f, object->GetPercentage());

		return repr;
	}

public:
	CapacitySettingUI(const StaticText32& name, Point2_i position, ICapacity* object, CapacitySettingsStyle style)
		: ISettingUI{ name, position }, object{ object }, style{ style }
	{
	}

	/* Обновляет текущее поле */
	void UpdateCurrentValue() override
	{
		if (IsCaptured()) return;

		//cur_idx = -1;
		//T get_value = cell->GetOrDefault();
		//for (int i = 0; i < (int)list.size(); i++)
		//{
		//	if (list[i].value == get_value)
		//	{
		//		cur_idx = i;
		//		break;
		//	}
		//}
	}

	//void Draw(IScreen& screen, Point2_i position) override
	//{
	//	if (name_value_offset < 0 || name_delim_offset < 0)
	//	{
	//		name_delim_offset = TextWidth(name);
	//		name_value_offset = TextWidth(name + ": ");
	//	}
	//
	//	DrawName(screen);
	//	DrawValue(screen);
	//}
};

#endif
