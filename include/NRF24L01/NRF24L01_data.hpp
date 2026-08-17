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
	extern LETO_CORE_EXPORT nRF24L01P_Scanner Scanner;

	extern LETO_CORE_EXPORT DataCell<uint8_t> Config;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Status;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Setup;
	extern LETO_CORE_EXPORT DataCell<uint8_t> FifoStatus;

	// REG: 0x04
	extern LETO_CORE_EXPORT DataCell<uint8_t> SetupRetr;

	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_00;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_01;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_02;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_03;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_04;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_05;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_06;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_07;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_08;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_09;
	
	extern LETO_CORE_EXPORT DataCell<uint64_t> Reg_0A;
	extern LETO_CORE_EXPORT DataCell<uint64_t> Reg_0B;
	
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_0C;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_0D;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_0E;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_0F;

	extern LETO_CORE_EXPORT DataCell<uint64_t> Reg_10;

	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_17;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_1C;
	extern LETO_CORE_EXPORT DataCell<uint8_t> Reg_1D;

	// nRF24L01+ version - true, nRF24L01 version - false
	extern LETO_CORE_EXPORT DataCell<bool> PVariant;

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
