/*
 * MoveAnimation.hpp
 *
 *  Created on: Mar 15, 2026
 *      Author: Timur
 */

#ifndef INC_UI_ANIMATION_MOVE_ANIMATION_HPP_
#define INC_UI_ANIMATION_MOVE_ANIMATION_HPP_

#include <UI/Animation/Animation.hpp>

#include <Drawable/IDrawable.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

class MoveAnimation : public Animation
{
protected:
	IDrawable& object;
	bool relative_duration;			// Изменяемая длительность анимации
	int speed;						// Скорость в пикселях в секунду
	Point2_i start;					// Начальное положение объекта
	Point2_i finish;				// Конечное положение объекта

	float CalcProgress(float t) override
	{
		return t;
	}

public:
	MoveAnimation(IDrawable& object, bool relative_duration, int value)
		: Animation{ relative_duration ? 0 : value }, 
		object{ object },
		relative_duration{ relative_duration }, speed { relative_duration ? value : 0 }
	{
		start = object.GetPosition();
		finish = object.GetPosition();
	}

	void SetStartPos(Point2_i start)
	{
		this->start = start;
		SetFinishPos(start);
		object.SetPosition(start);
	}

	void SetFinishPos(Point2_i finish)
	{
		this->finish = finish;
	}

	void Play() override
	{
		/// TODO: нужно ли так плотно связывать работу MoveAnimation с LetoAPI?
		start = object.GetPosition();
		if (relative_duration && speed)
			duration_ms = static_cast<int>(1000.f * leto_api_v1->Math->sqrtf(
				1.0f * (finish.x - start.x) * (finish.x - start.x) + 
				1.0f * (finish.y - start.y) * (finish.y - start.y)
			) / (float) speed);
		Animation::Play();
	}

	void Tick() override
	{
		int x = start.x + static_cast<int>(GetProgress() * (finish.x - start.x));
		int y = start.y + static_cast<int>(GetProgress() * (finish.y - start.y));
		object.SetPosition({x, y});
	}

};

#endif