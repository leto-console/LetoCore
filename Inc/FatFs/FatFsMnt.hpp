/**
 * @file FatFsMnt.hpp
 * @date May 31, 2026
 * @author Rakhimov T.
 */

#ifndef INC_FATFS_MNT_HPP_
#define INC_FATFS_MNT_HPP_

#include "LibrariesExport.h"

#include <FatFs/low_level/ff.h>

/// Примонтированная файловая система
extern LETO_CORE_EXPORT FATFS fatfs;

/// Флаг примонтированности файловой системы
extern LETO_CORE_EXPORT bool fatfs_mounted; 

/// Флаг первой проверки файловой системы
extern LETO_CORE_EXPORT bool fatfs_first_check;

#endif
