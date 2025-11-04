/*
 * IBinaryStorage.hpp
 *
 *  Created on: Dec 12, 2025
 *      Author: Timur
 */

#ifndef INC_STORAGE_I_BINARY_STORAGE_HPP_
#define INC_STORAGE_I_BINARY_STORAGE_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

class LIBRARIES_EXPORT IBinaryStorage
{
public:
	virtual ~IBinaryStorage() = default;

	virtual void Write(uint16_t startByte, const void* data, uint16_t length) = 0;
	virtual void Read(uint16_t startByte, void* data, uint16_t length) const = 0;
};

#endif
