/**
 * @file DebugMode.hpp
 * @date Mar 28, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_DEBUG_MODE_HPP_
#define INC_SYSTEM_DEBUG_MODE_HPP_

#include "LibrariesExport.h"

#include <Data/IDataCell.hpp>

/**
 * @brief Проинициализировать ячейку с флагом отладочного режима
 */
extern LIBRARIES_EXPORT void InitDebugModeCell(IDataCell<bool>* cell);

/**
 * @brief Получить отладочный режим работы системы
 * @return `true` - включен, `false` - выключен
 */
extern LIBRARIES_EXPORT bool GetDebugMode();

#endif
