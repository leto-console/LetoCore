/**
 * @file LetoApplication.hpp
 * @date May 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_APPLICATION_BASE_APPLICATION_ABI_HPP_
#define INC_APPLICATION_BASE_APPLICATION_ABI_HPP_

#include <cstdint>

#include <ABI/AppBinHeader.hpp>
#include <ABI/AppEvent.hpp>
#include <LetoAPI_V1/LetoAPI_V1.hpp>
#include <LetoAPI_V1/Application/LetoAppStatus.hpp>
#include <LetoAPI_V1/Application/LetoResult.hpp>

#include <Graphics/IScreen.hpp>

#pragma pack(push, 4)

/**
 * @brief Приложение Leto OS
 */
struct LetoApplication_V1
{
	/// Заголовок исполняемого приложения
	AppBinHeader Header;

	/**
	 * @brief Проинициализировать приложение
	 */
	LetoResult_V1 (*Init) (const LetoAPI_V1* api);
	
	/**
	 * @brief Очистить приложение
	 */
	LetoResult_V1 (*Clean) ();

	/**
	 * @brief Получить статус работы приложения
	 */
	LetoAppStatus_V1 (*GetStatus) ();
	
	/**
	 * @brief Обработать событие
	 *
	 * @return 
	 * - `APP_SUCCESS` событие обработано
	 * 
	 * - `APP_UNSUPPORT` событие не обрабатывается приложением
	 */
	LetoResult_V1 (*ProcessEvent) (AppEvent event);
	
	/**
	 * @brief Отрисовать приложение
	 */
	LetoResult_V1 (*Draw) (IScreen* screen);
	
	/**
	 * @brief Выполнить действия в фоне
	 */
	LetoResult_V1 (*Loop) ();
	
};

#pragma pack(pop)

static_assert(std::is_standard_layout<LetoApplication_V1>::value, "LetoApplication_V1 должен быть standard_layout");

#endif
