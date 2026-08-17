#include <NRF24L01/NRF24L01_data.hpp>

#include <Data/DataCell.hpp>

namespace NRF24L01
{
	nRF24L01P_Scanner Scanner;

	DataCell<uint8_t> Reg_00;
	DataCell<uint8_t> Reg_01;
	DataCell<uint8_t> Reg_02;
	DataCell<uint8_t> Reg_03;
	DataCell<uint8_t> Reg_04;
	DataCell<uint8_t> Reg_05;
	DataCell<uint8_t> Reg_06;
	DataCell<uint8_t> Reg_07;
	DataCell<uint8_t> Reg_08;
	DataCell<uint8_t> Reg_09;

	DataCell<uint64_t> Reg_0A;
	DataCell<uint64_t> Reg_0B;
	
	DataCell<uint8_t> Reg_0C;
	DataCell<uint8_t> Reg_0D;
	DataCell<uint8_t> Reg_0E;
	DataCell<uint8_t> Reg_0F;

	DataCell<uint64_t> Reg_10;

	DataCell<uint8_t> Reg_17;
	DataCell<uint8_t> Reg_1C;
	DataCell<uint8_t> Reg_1D;


	DataCell<bool> PVariant;
	DataCell<uint8_t> Config;
	DataCell<uint8_t> Status;
	DataCell<uint8_t> Setup;

	DataCell<uint8_t> SetupRetr;

	DataCell<uint8_t> FifoStatus;
}
