/**
 * @file ICapacity.hpp
 * @date Mar 08, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_I_CAPACITY_HPP_
#define INC_DATA_I_CAPACITY_HPP_

// Интерфейс для мониторинга занятости ресурса
class ICapacity
{
public:
	virtual ~ICapacity() = default;

	// Получить занятое количество байтов ресурса
	virtual int GetUsedBytes() const = 0;

	// Получить общее количество байтов ресурса
	virtual int GetTotalBytes() const = 0;

	// Получить процент загрузки
	float GetPercentage() const
	{
		if (GetTotalBytes() == 0)
			return 0.0f;
		return 1.0f * GetUsedBytes() / GetTotalBytes();
	}

	// Получить свободное количество байтов ресурса 
	int GetFreeBytes() const
	{
		return GetTotalBytes() - GetUsedBytes();
	}

};

#endif