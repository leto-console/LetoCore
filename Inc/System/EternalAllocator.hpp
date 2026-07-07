/**
 * @file EternalAllocator.hpp
 * @date Jul 06, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_ETERNAL_ALLOCATOR_HPP_
#define INC_SYSTEM_ETERNAL_ALLOCATOR_HPP_

#include "LibrariesExport.h"

#include <Data/ArenaAllocator.hpp>

#ifndef ETERNAL_ALLOCATOR_SIZE
#define ETERNAL_ALLOCATOR_SIZE (1 * 1024)
#endif

/// @brief Вечный аллокатор (без возможности удаления, только для аллокации)
extern LIBRARIES_EXPORT ArenaAllocator<ETERNAL_ALLOCATOR_SIZE, false> EternalAllocator;

#endif
