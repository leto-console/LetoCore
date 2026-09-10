/*
 * BaseGame.hpp
 *
 *  Created on: Dec 06, 2025
 *      Author: Timur
 */

#ifndef INC_GAMES_BASE_GAME_HPP_
#define INC_GAMES_BASE_GAME_HPP_

#include "LibrariesExport.h"

#include <Data/RingFIFO_Static.hpp>
#include <Data/IAllocator.hpp>
#include <Data/StaticText.hpp>

#include <LetoABI/AppBinHeader.h>
#include <LetoABI/AppEvent.h>

#include <LetoAPI_V1/LetoAPI_V1.h>
#include <SceneManager/SceneManager.hpp>

#include <UI/CapacityIndicatorUI.hpp>


extern LETO_CORE_EXPORT ISceneManager* LoadedGame; ///< Loaded game instance
extern LETO_CORE_EXPORT const LetoAllocator_V1* LoadedGameAllocator();

 // Интерфейс базовой игры
template <uint32_t TScenesMaxCount = 32, uint32_t TBuilderAllocSize = 512, uint32_t TCommonAllocSize = 14 * 1024>
class BaseGame : public SceneManager<TScenesMaxCount, TBuilderAllocSize, TCommonAllocSize>
{
protected:
	/// Заголовок игры с общей информацией
	AppBinHeader header;

	// Флаг, что игра закрыта
	bool close_flag = false;

	virtual bool CustomInit() { return true; };
	virtual void CustomClose() {};

public:
	BaseGame(const AppBinHeader& header) : header{ header } { LoadedGame = this; }
	virtual ~BaseGame() { LoadedGame = nullptr; Close(); }

	uint16_t GetID() const { return header.id; }

	// Признак завершения игры
	bool IsClosed() const { return close_flag; };

	// Инициализация игры
	bool Init()
	{
		close_flag = false;
		return CustomInit();
	}

	// Функция завершения игры
	void Close()
	{
		close_flag = true;
		SceneManager<TScenesMaxCount, TBuilderAllocSize, TCommonAllocSize>::ClearScenes();
		CustomClose();
	}

	void Draw(IScreen& screen) override
    {
		SceneManager<TScenesMaxCount, TBuilderAllocSize, TCommonAllocSize>::Draw(screen);

		if (leto_api_v1->Globals->GetDebugMode())
		{
			Point2_i p{24, 128-24};
			{
				static CapacityIndicatorUI capacity {p, {20, 5}, 
					&SceneManager<TScenesMaxCount, TBuilderAllocSize, TCommonAllocSize>::GetCommonAllocator(), "C"};
				capacity.Draw(screen);
			}
			{
				static CapacityIndicatorUI capacity {p + Point2_i{30,0}, {20, 5}, 
					&SceneManager<TScenesMaxCount, TBuilderAllocSize, TCommonAllocSize>::GetSceneAllocator(), "B"};
				capacity.Draw(screen);
			}
		}
    }
};

#endif
