/**
 * @file CapacityIndicatorUI
 * @date Mar 26, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_CAPACITY_INDICATOR_UI_HPP_
#define INC_UI_CAPACITY_INDICATOR_UI_HPP_

#include <Drawable/IDrawable.hpp>
#include <Data/ICapacity.hpp>
#include <SceneManager/ISceneObject.hpp>
#include <UI/Text/UI_Label.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

#include <DrawFunctions/DrawRectangle.hpp>
#include <DrawFunctions/DrawText.hpp>

#include <LetoAPI_V1/LetoAPI_V1.h>

class CapacityIndicatorUI : public virtual ISceneObject
{
protected:
	UI_Label ui_name;
	UI_Label ui_value;

	bool horizontal = true;		///< Горизонтальность индикатора

	ICapacity* object;			///< Отображаемый объект

public:
	CapacityIndicatorUI(
		Point2_i position, 
		Point2_i size, 
		ICapacity* object, 
		const StaticText32& name, 
		bool horizontal = true)
		: object{ object }, horizontal{ horizontal }
	{
		SetPosition(position);
		SetWidth(size.x);
		SetHeight(size.y);
//
		ui_name.SetText(name);
		ui_name.SetFont(IFont::FromHandle(leto_api_v1->Font->GetFont(7, 7, 1)));
		ui_name.Enable();
//
		ui_value.SetText("");
		ui_value.SetFont(IFont::FromHandle(leto_api_v1->Font->GetFont(7, 7, 1)));
		ui_value.Enable();
	}

	bool ProcessInput(const AppEvent& event) override { return false; }

	/* Обновляет текущее поле */
	void Loop() override
	{
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

	void Draw(IScreen& screen, Point2_i offset = {}) override
	{
		using namespace DrawFunctions;

		float perc = object->GetPercentage();

		Point2_i p = position;

		ui_name.SetPosition(p);
		p.x += (TextWidth(ui_name.GetText(), ui_name.GetFont()) + 1);
		ui_value.SetPosition(p - Point2_i{0, 7});
		p.y += 1;

		static StaticText32 text{};
		leto_api_v1->Text->FormatText(text.CharPtr(), text.Capacity(), "%d", static_cast<int>(100 * perc));
		ui_value.SetText(text);

		ui_name.MainDraw(screen);
		ui_value.MainDraw(screen);

		Point2_i lu = p + offset;
		Point2_i rd = lu + size;
	
		DrawOutlinedRectangle(screen, lu, rd, BlackColor, WhiteColor);

		lu += {2, 2};
		rd -= {2, 2};

		if (horizontal)
		{
			int width = rd.x - lu.x;
			if (width < 0)
				return;
			
			rd.x = lu.x + static_cast<int>(leto_api_v1->Math->roundf(perc * width));
			if (rd.x == lu.x) return;
		}
		else 
		{
			int height = rd.y - lu.y;
			if (height < 0)
				return;
			
			rd.y = lu.y + static_cast<int>(leto_api_v1->Math->roundf(perc * height));
			if (rd.y == lu.y) return;
		}

		DrawRectangle(screen, lu, rd, WhiteColor);
	}
};

#endif
