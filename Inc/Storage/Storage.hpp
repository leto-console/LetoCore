/*
 * SettingStorage.hpp
 *
 *  Created on: Dec 12, 2025
 *      Author: Timur
 */

#ifndef INC_SETTINGS_SETTINGS_STORAGE_HPP_
#define INC_SETTINGS_SETTINGS_STORAGE_HPP_

#include "LibrariesExport.h"
#include "IBinaryStorage.hpp"

#include <EEPROM/EEPROM_24C.hpp>

class LETO_CORE_EXPORT Storage : public IBinaryStorage
{
protected:
	EEPROM_24C* eeprom{ nullptr };

public:
	Storage();

	void Init(EEPROM_24C& eeprom);

	void Write(uint16_t startByte, const void* data, uint16_t length) override;
	void Read(uint16_t startByte, void* data, uint16_t length) const override;
};

#endif
