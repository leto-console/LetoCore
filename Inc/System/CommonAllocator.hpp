/**
 * @file CommonAllocator.hpp
 * @date May 22, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_COMMON_ALLOCATOR_HPP_
#define INC_SYSTEM_COMMON_ALLOCATOR_HPP_

#include "LibrariesExport.h"

#include <Data/ArenaAllocator.hpp>

#ifndef COMMON_ALLOCATOR_SIZE
#define COMMON_ALLOCATOR_SIZE (20 * 1024)
#endif

/// @brief Аллокатор общего назначения (для сцен, игр и т.д.)
extern LIBRARIES_EXPORT ArenaAllocator<COMMON_ALLOCATOR_SIZE> CommonAllocator;

#endif
