/*
 * Animation.hpp
 *
 *  Created on: Dec 09, 2025
 *      Author: Timur
 */

#ifndef INC_UI_ANIMATION_ANIMATION_HPP_
#define INC_UI_ANIMATION_ANIMATION_HPP_

#include "LibrariesExport.h"

#include <Graphics/IScreen.hpp>

#include <LetoAPI_V1/LetoAPI_V1.h> 

#include <cstdint>

// Интерфейс анимации
class LETO_CORE_EXPORT Animation
{
protected:
	enum class State { PLAY, PAUSE, STOP };
	State state = State::STOP;

	// Сброс состояния до начального при завершении анимации
	bool reset_to_init{};

	// Время начала проигрывания анимации
	int start_ms{};

	// Время начала проигрывания анимации
	int pause_ms{};

	// Продолжительность анимации
	int duration_ms{};

	// Функция, которая считает функцию f(time) = animation (базово - линейно)
	virtual float CalcProgress(float t)
	{
		return t;
	}

	// Получить прогресс выполнения анимации от 0.0 до 1.0 (базово - линейная)
	virtual float GetProgress()
	{
		if (state == State::STOP)
			return reset_to_init ? 0.0f : 1.0f;

		int cur_ms = leto_api_v1->Globals->GetCurrentMs();
		if (state == State::PAUSE) cur_ms = pause_ms;
		
		float t = 1.0f * (cur_ms - start_ms) / duration_ms;
		float progress = CalcProgress(t);
		if (t > 1.0f)
		{
			progress = 1.0f;
			state = State::STOP;
		}

		return progress;
	}

public:
	Animation(int duration_ms);

	virtual bool IsPlaying() const;

	// Включение анимации (или возобновление)
	virtual void Play();

	// Пауза анимации
	virtual void Pause()
	{
		pause_ms = leto_api_v1->Globals->GetCurrentMs();
		state = State::PAUSE;
	}

	// Выключение анимации
	virtual void Stop()
	{
		state = State::STOP;
	}

	// Задать сброс состояния анимации до исходной при завершении анимации (по умолчанию нет)
	virtual void SetResetToStart(bool reset_to_init)
	{
		this->reset_to_init = reset_to_init;
	}

	virtual void Tick() = 0;
};

#endif
