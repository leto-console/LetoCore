/*
 * StoredDataCell.hpp
 *
 *  Created on: Dec 12, 2025
 *      Author: Timur
 */

#ifndef INC_SETTINGS_STORED_DATA_CELL_HPP_
#define INC_SETTINGS_STORED_DATA_CELL_HPP_

#include <stdint.h>
#include <Storage/Storage.hpp>
#include <cstring>

#include <Data/IDataCell.hpp>


// Класс для настройки какого-либо параметра
template <typename T>
class StoredDataCell : public IDataCell<T>
{
protected:
	// Текущее значение хранимой в памяти настройки
	mutable T value{};

	// Флаг инициализации
	mutable bool inited{ false };

	// Адрес настройки в памяти
	uint16_t address;

	// Держатель настроек в памяти
	Storage* storage;

public:
	StoredDataCell(uint16_t address, Storage* storage)
		: address { address }, storage{ storage }
	{
	}

	uint16_t GetAddress() const { return address; }

	void Clear()
	{
		memset(&value, 0, sizeof(T));
		Set(value);
	}

	// Установить значение в eeprom
	void Set(const T& value) override
	{
		this->value = value;
		storage->Write(address, &value, sizeof(T));
	}

	// Получить значение из eeprom
	bool Get(T& value) const override
	{
		if (!inited)
		{
			storage->Read(address, &(this->value), sizeof(T));
			inited = true;
		}
		value = this->value;
		return true;
	}

	T GetOrDefault() const override
	{
		T value{};
		Get(value);
		return value;
	}
};

template <>
class StoredDataCell<bool> : public IDataCell<bool>
{
protected:
	// Текущее значение хранимой в памяти настройки
	mutable bool value{};

	// Флаг инициализации
	mutable bool inited{ false };

	// Адрес байта настройки в памяти
	uint16_t address;

	// Бит в байте настройки в памяти
	uint8_t bit;

	// Держатель настроек в памяти
	Storage* storage;

public:
	StoredDataCell(uint16_t address, uint8_t bit, Storage* storage)
		: address{ address }, bit{ bit }, storage{ storage }
	{
	}

	void Clear()
	{
		memset(&value, 0, 1);
		Set(value);
	}

	// Установить значение в eeprom
	void Set(const bool& value) override
	{
		this->value = value;

		uint8_t byte{};
		storage->Read(address, &byte, 1);

		if (value)
			byte |= (1 << bit);
		else
			byte &= ~(1 << bit);

		storage->Write(address, &byte, 1);
	}

	// Получить значение из eeprom
	bool Get(bool& value) const override
	{
		if (!inited)
		{
			uint8_t byte{};
			storage->Read(address, &byte, 1);
			this->value = (byte >> bit) & 0x1;
			inited = true;
		}
		value = this->value;
		return true;
	}

	bool GetOrDefault() const override
	{
		bool value{};
		Get(value);
		return value;
	}
};

#endif
