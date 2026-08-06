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

 // Интерфейс базовой игры
template <uint32_t TScenesMaxCount = 32, uint32_t TBuilderAllocSize = 512>
class LETO_CORE_EXPORT BaseGame : public SceneManager<TScenesMaxCount, TBuilderAllocSize>
{
protected:
	/// Заголовок игры с общей информацией
	AppBinHeader header;

	// Флаг, что игра закрыта
	bool close_flag = false;

	virtual bool CustomInit() {};
	virtual void CustomClose() {};

public:
	BaseGame(const AppBinHeader& header) : header{ header } { }
	virtual ~BaseGame() { Close(); }

	uint16_t GetID() const { return header.id; }

	// Признак завершения игры
	bool IsClosed() const { return close_flag; };

	// Инициализация игры
	bool Init()
	{
		close_flag = false;
		CustomInit();
	}

	// Функция завершения игры
	void Close()
	{
		close_flag = true;
		SceneManager<TScenesMaxCount, TBuilderAllocSize>::ClearScenes();
		CustomClose();
	}
};

#endif
