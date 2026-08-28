/*
 * NamedDataCell.hpp
 *
 *  Created on: Feb 08, 2026
 *      Author: Timur
 */

#ifndef INC_SETTINGS_NAMED_DATA_CELL_HPP_
#define INC_SETTINGS_NAMED_DATA_CELL_HPP_

#include <Storage/NamedSpace.hpp>

#include <Data/IDataCell.hpp>
#include <Data/StaticText.hpp>

// Именованная ячейка - для хранения настройки в именованной части памяти
template <typename T>
class NamedDataCell : public IDataCell<T>
{
protected:
	// Название ячейки
	StaticText32 name;

	// Указатель на указатель с пространством (для возможности динамически изменить пространство для конкретной переменной)
	NamedSpace** space_switcher{ nullptr };

	// Указатель на пространство
	mutable NamedSpace* space{ nullptr };

	// Готовность пространства
	bool SpaceReady() const
	{
		if (space_switcher && *space_switcher)
			space = *space_switcher;

		return space;
	}

public:
	NamedDataCell(const StaticText32& name, NamedSpace** space_switcher)
		: name{ name }, space_switcher{ space_switcher }
	{
	}

	NamedDataCell(const StaticText32& name, NamedSpace* space)
		: name{ name }, space{ space }
	{
	}

	// Установить значение в eeprom
	void Set(const T& value) override
	{
		if (!SpaceReady())
			return;

		if (!space->Exists(name))
			space->Create(name, value);
		else
			space->Update(name, value);
	}

	// Получить значение из eeprom
	bool Get(T& value) const override
	{
		if (!SpaceReady())
			return false;

		return space->Read(name, value);
	}

	T GetOrDefault() const override
	{
		T value{};

		if (SpaceReady())
			space->Read(name, value);

		return value;
	}
};

#endif
