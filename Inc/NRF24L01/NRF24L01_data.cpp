#include <NRF24L01/NRF24L01_data.hpp>

#include <Data/DataCell.hpp>

namespace NRF24L01
{
	nRF24L01P_Scanner Scanner;

	DataCell<bool> PVariant;
	DataCell<uint8_t> Config;
	DataCell<uint8_t> Status;
	DataCell<uint8_t> Setup;

	DataCell<uint8_t> SetupRetr;

	DataCell<uint8_t> FifoStatus;
}
