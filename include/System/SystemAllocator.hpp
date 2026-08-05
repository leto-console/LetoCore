/**
 * @file SystemAllocator.hpp
 * @date May 21, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_SYSTEM_ALLOCATOR_HPP_
#define INC_SYSTEM_SYSTEM_ALLOCATOR_HPP_

#include "LibrariesExport.h"

#include <Data/ArenaAllocator.hpp>

#ifndef SYSTEM_ALLOCATOR_SIZE
#define SYSTEM_ALLOCATOR_SIZE (2 * 1024)
#endif

/// @brief Системный аллокатор (для билдеров объектов, задач и прочего)
extern LETO_CORE_EXPORT ArenaAllocator<SYSTEM_ALLOCATOR_SIZE> SystemAllocator;

#endif
