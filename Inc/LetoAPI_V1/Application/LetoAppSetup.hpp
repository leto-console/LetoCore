/**
 * @file LetoEvent.hpp
 * @date May 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_APPLICATION_LETO_APP_SETUP_HPP_
#define INC_LETO_API_V1_APPLICATION_LETO_APP_SETUP_HPP_

#include "LibrariesExport.h"

#include <LetoAPI_V1/Application/LetoResult.hpp>
#include <LetoAPI_V1/LetoAPI_V1.hpp>

/**
 * @brief Функция глобальной инициализации бинарного приложения
 */
extern LIBRARIES_EXPORT LetoResult_V1 LetoAppSetup(const LetoAPI_V1* api);

#endif
