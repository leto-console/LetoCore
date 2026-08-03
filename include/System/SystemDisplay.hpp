/**
 * @file SystemDisplay.hpp
 * @date Jul 01, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_SYSTEM_DISPLAY_HPP_
#define INC_SYSTEM_SYSTEM_DISPLAY_HPP_

#include "LibrariesExport.h"

#include <Graphics/IDisplay.hpp>
#include <System/StackGuard.hpp>

/// @brief Системный дисплей (доступен отовсюду)
extern LETO_CORE_EXPORT IDisplay* SystemDisplay;

/// @brief Разрушение его лишает возможности отрисовывать что-либо на дисплее
extern LETO_CORE_EXPORT StackGuard* DisplayStackGuard;

#endif
