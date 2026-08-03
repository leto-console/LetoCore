/*
 *	IScreen.hpp
 *
 *	Created on: Mar 19, 2026
 *		Author: Timur
 */

#ifndef INC_GRAPHICS_ISCREEN_FEATURES_HPP_
#define INC_GRAPHICS_ISCREEN_FEATURES_HPP_

#include <Graphics/RGBColor.hpp>
#include <AbstractCalls/CallStruct.hpp>

// Разные доп. функции, которые могут быть аппаратно реализованы драйвером дисплея
namespace IScreenFeatures
{	
	enum : uint32_t
	{
		__START_ID,
		DrawLine_ID,	// Отрисовать линию
		FillRect_ID,	// Заполнить прямоугольник
		
	};
			
	// ----------------------------------------------------------------------------------------------------
	//  Структуры вызовов
	// ----------------------------------------------------------------------------------------------------
	
	// Получить текущее пространство
	struct __DrawLine_Data__
	{		
		struct Arg { int x0; int y0; int x1; int y0; RGBColor color; };
		struct Answer { };
	};
	struct DrawLine : public CallStruct<__DrawLine_Data__, DrawLine_ID> { };
	
};


#endif
