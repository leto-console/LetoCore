/*
 * NamedSpace.hpp
 *
 *  Created on: Feb 08, 2026
 *      Author: Timur
 */

#ifndef INC_SETTINGS_NAMED_SPACE_HPP_
#define INC_SETTINGS_NAMED_SPACE_HPP_

#include "LibrariesExport.h"

#include <stdint.h>
#include <Storage/IBinaryStorage.hpp>
#include <Data/ICapacity.hpp>
#include <Data/StaticText.hpp>
#include <Data/StaticList.hpp>

constexpr uint16_t DATA_NAME_SIZE = 8;

#define NAMED_DATA_HEADER_SIGNATURE { '\xC1', '\xA0' }

struct LETO_CORE_EXPORT NamedDataHeader
{
	const char SIGNATURE[2] = NAMED_DATA_HEADER_SIGNATURE;

	// Название настройки
	char name[DATA_NAME_SIZE]{};

	// Размер настройки
	uint16_t size{};
};

// Область памяти, хранящая именованные настройки
class LETO_CORE_EXPORT NamedSpace : public ICapacity
{
protected:
	struct ScanRecord
	{
		uint16_t					address{};
		StaticText<DATA_NAME_SIZE>	name{};
		uint16_t					size{};

		ScanRecord() = default;

		ScanRecord(uint16_t address, const StaticText32& name, uint16_t size)
			: address{ address }, name{ name }, size{ size }
		{
		}

		// Возвращает адрес первого байта данных
		uint16_t DataAddress() const
		{
			// field.SIGN + field.NAME + field.SIZE
			return address + (2 + DATA_NAME_SIZE + 2);
		}

		// Возвращает адрес первого свободного байта после записи
		uint16_t NextAddress() const
		{
			return DataAddress() + size;
		}
	};

	// Адрес начала области памяти
	const uint16_t start_address;

	// Размер области памяти
	const uint16_t space_size;

	// Хранилище данных
	IBinaryStorage* storage;

	// Список записей объектов в области памяти
	StaticList<ScanRecord, 32> records;

	// Найти запись по имени
	bool FindRecordByName(const StaticText32& name, ScanRecord& record) const;

	/* Операции управления именованной памятью */

	// Найти свободный незанятый участок памяти 
	bool Allocate(uint16_t& addr_lnk, const StaticText32& name, const NamedDataHeader& data);

	// Освободить занятый участок памяти
	bool Free(const StaticText32& name);

	/* Операции с указателями void* */
	
	// Добавить абстрактные данные по указателю на const void* и размеру
	bool Create(const StaticText32& name, const void* data, uint16_t data_size);

	// Получить абстрактные данные по указателю на const void* и размеру
	bool Read(const StaticText32& name, void* data, uint16_t data_size);

	// Отредактировать абстрактные данные по указателю на const void* и размеру
	bool Update(const StaticText32& name, const void* data, uint16_t data_size);

public:
	NamedSpace(uint16_t start_address, uint16_t space_size, IBinaryStorage* storage);

	// Получить занятое количество байтов ресурса
	int GetUsedBytes() const override;

	// Получить общее количество байтов ресурса
	int GetTotalBytes() const override;

	// Очистить область памяти
	void Clear();
	
	// Просканировать пространство на наличие записей
	void Scan();

	// Запись существует
	bool Exists(const StaticText32& name) const;

	// Создать запись с данными (при отстутствии)
	template <typename Data>
	bool Create(const StaticText32& name, Data& data)
	{
		return Create(name, &data, sizeof(data));
	}

	// Прочитать запись с данными (при наличии)
	template <typename Data>
	bool Read(const StaticText32& name, Data& data)
	{
		return Read(name, &data, sizeof(data));
	}

	// Обновить запись с данными (при наличии)
	template <typename Data>
	bool Update(const StaticText32& name, Data& data)
	{
		return Update(name, &data, sizeof(data));
	}

	// Удалить запись с данными (при наличии)
	void Delete(const StaticText32& _name);

};

#endif