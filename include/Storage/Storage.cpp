/*
 * Storage.cpp
 *
 *  Created on: Dec 12, 2025
 *      Author: Timur
 */

#include <Storage/Storage.hpp>

Storage::Storage()
{
}

void Storage::Init(EEPROM_24C& eeprom)
{
	this->eeprom = &eeprom;
}

// TODO: Аналогично SettingsStorage::Read
void Storage::Write(uint16_t startByte, const void* data, uint16_t length)
{
	if (eeprom) eeprom->Write(0, startByte, data, length);
}

// TODO: Сделать проверку на отказ EEPROM и возврат true или false по результатам чтения
void Storage::Read(uint16_t startByte, void* data, uint16_t length) const
{
	if (eeprom) eeprom->Read(0, startByte, data, length);
}
