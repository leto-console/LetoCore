/*
 * NRF24L01_data.hpp
 *
 *  Created on: Dec 19, 2025
 *      Author: Timur
 */

#ifndef INC_NRF24L01_DATA_HPP_
#define INC_NRF24L01_DATA_HPP_

#include "LibrariesExport.h"

#include <Data/DataCell.hpp>
#include <NRF24L01/nRF24L01P_Scanner.hpp>

#include <stdint.h>

namespace NRF24L01
{
	extern LIBRARIES_EXPORT nRF24L01P_Scanner Scanner;

	extern LIBRARIES_EXPORT DataCell<uint8_t> Config;
	extern LIBRARIES_EXPORT DataCell<uint8_t> Status;
	extern LIBRARIES_EXPORT DataCell<uint8_t> Setup;
	extern LIBRARIES_EXPORT DataCell<uint8_t> FifoStatus;

	// REG: 0x04
	extern LIBRARIES_EXPORT DataCell<uint8_t> SetupRetr;

	// nRF24L01+ version - true, nRF24L01 version - false
	extern LIBRARIES_EXPORT DataCell<bool> PVariant;

	const uint32_t PipeAddress[]
	{
		0x78787878,
		0xABCDEF0A,		// адрес первой трубы
		0xABCDEF0B,
		0xABCDEF0C,
		0xABCDEF0D,
		0xABCDEF0E,		// адрес пятой трубы
	};
};

#endif
