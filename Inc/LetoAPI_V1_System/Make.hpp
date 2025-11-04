/**
 * @file Init_V1.hpp
 * @date May 23, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_MAKE_HPP_
#define INC_LETO_API_V1_MAKE_HPP_

#include "LibrariesExport.h"

#include <LetoAPI_V1/LetoAPI_V1.hpp>

/**
 * @brief Создать и проинициализировать системный API
 * 
 * Создание объекта системного API может быть произведено только в самой системе
 * 
 * @warning Вызов данной функции в приложении приведет к UB
 */
extern LIBRARIES_EXPORT const LetoAPI_V1* Make_LetoAPI_V1();

/// Автоматическое создание API при инициализации статических переменных
static struct LetoAPI_V1_StaticMaker
{
    LetoAPI_V1_StaticMaker() { Make_LetoAPI_V1(); }
} 
__LetoAPI_V1_Static_Maker__;

#endif
