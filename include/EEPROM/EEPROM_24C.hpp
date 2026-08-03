/*
 * EEPROM_23C.hpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#ifndef INC_EEPROM_EEPROM_24C_HPP_
#define INC_EEPROM_EEPROM_24C_HPP_

#ifdef USE_HAL_DRIVER

#include <HAL_include/HAL.hpp>

#else

#include <string>

#endif

#ifdef _WIN32
#include <cstdio>
#include <stdexcept>
#include <cstring>
#endif

#include "LibrariesExport.h"

#include <cstdint>

class LETO_CORE_EXPORT EEPROM_24C
{
protected:
#ifdef USE_HAL_DRIVER
	I2C_HandleTypeDef* hi2c;
#else
	uint8_t* memory{};
	std::string filename;

	FILE* file_handle{};
#endif
	
	uint8_t addr_size;
	uint32_t size_bytes;	// Размер EEPROM в байтах

	void SyncFile() const;

public:
#ifdef USE_HAL_DRIVER
	EEPROM_24C(I2C_HandleTypeDef* hi2c, uint32_t size_bytes);
#else 
	EEPROM_24C(uint32_t size_bytes, const std::string& filename);
	~EEPROM_24C();
#endif

	// Размер EEPROM памяти в байтах
	uint32_t GetSizeBytes() const;

	void Write(uint8_t page, uint16_t startByte, const void* data, uint16_t length);
	void Read(uint8_t page, uint16_t startByte, void* data, uint16_t length) const;
};

#endif /* INC_EEPROM_EEPROM_24C_08_HPP_ */
