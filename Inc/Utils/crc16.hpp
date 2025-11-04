#ifndef INC_UTILS_CRC16_HPP
#define INC_UTILS_CRC16_HPP

#include "LibrariesExport.h"

#include <stdint.h>
#include <stddef.h>

LIBRARIES_EXPORT uint16_t calc_crc16(const void* buf, size_t len);

#endif 
