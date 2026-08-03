/*
 * LambdaAnimation.hpp
 *
 *  Created on: Mar 15, 2026
 *      Author: Timur
 */

#ifndef INC_UI_ANIMATION_LAMBDA_ANIMATION_HPP_
#define INC_UI_ANIMATION_LAMBDA_ANIMATION_HPP_

#include "LibrariesExport.h"
#include <UI/Animation/Animation.hpp>

template <typename Object>
class LETO_CORE_EXPORT LambdaAnimation : public Animation
{
public:
	// Функция должна принимать 2 аргумента: Object& и float - 0.0...1.0 - прогресс выполнения анимации
	using TickFunc = void(*)(Object&, float);

protected:
	// Объект на которых воздействует анимация
	Object* object{};

	// Функция с помощью которой воздействуется на объект
	TickFunc func{};

	float CalcProgress(float progress) override
	{
		return progress;
	}

public:
	LambdaAnimation(int duration_ms) : Animation{ duration_ms }
	{
	}

	void Init(Object* object, TickFunc func)
	{
		this->object = object;
		this->func = func;
	}

	void Tick() override
	{
		if (!func || !object)
			return;
		func(*object, GetProgress());
	}
};

#endif