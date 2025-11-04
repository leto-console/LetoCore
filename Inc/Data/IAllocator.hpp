/**
 * @file IAllocator.hpp
 * @date Mar 05, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_I_ALLOCATOR_HPP_
#define INC_DATA_I_ALLOCATOR_HPP_

#include <stdint.h>
#include <utility>
#include <new>

#include <Data/ICapacity.hpp>

class IAllocator : public ICapacity
{
public:
	virtual ~IAllocator() = default;

	template <typename Type, typename... Args>
	Type* Make(Args... args)
	{
		void* ptr = Alloc(sizeof(Type));
		if (!ptr)
			return nullptr;
		return new(ptr) Type(std::forward<Args>(args)...);
	}

	virtual void* Alloc(uint32_t size) = 0;
	virtual void Free(const void* ptr) = 0;

	/**
	 * @brief Очистить память от указателя до конца памяти
	 */
	virtual void Clear(const void* ptr) = 0;

	/**
	 * @brief Очистить память
	 */
	virtual void Clear() = 0;

	// Получить занятое количество байтов ресурса
	virtual int GetUsedBytes() const = 0;

	// Получить общее количество байтов ресурса
	virtual int GetTotalBytes() const = 0;
};

#endif
