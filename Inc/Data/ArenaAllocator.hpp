/**
 * @file ArenaAllocator.hpp
 * @date Mar 05, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_ARENA_ALLOCATOR_HPP_
#define INC_DATA_ARENA_ALLOCATOR_HPP_

#include <stdint.h>
#include <cstring>

#include <Data/IAllocator.hpp>

template <int ArenaSize>
class ArenaAllocator : public IAllocator
{
protected:
	uint8_t data[ArenaSize];
	uint32_t _inner_idx{};

public:
	// Получить занятое количество байтов ресурса
	int GetUsedBytes() const override
	{
		return _inner_idx;
	}

	// Получить общее количество байтов ресурса
	int GetTotalBytes() const override
	{
		return ArenaSize;
	}

	void* Alloc(uint32_t size) override
	{
		/*
			Не скупись, выдели больше памяти
		*/
		if (_inner_idx + size + sizeof(uint32_t) > ArenaSize)
		{
#ifdef WIN32
			throw "Bad_alloc";
#else
			return nullptr;
#endif
		}

		memcpy(&data[_inner_idx], &size, sizeof(size));
		_inner_idx += sizeof(uint32_t);

		void* ptr = &data[_inner_idx];
		_inner_idx += size;
		return ptr;
	}

	void Free(const void* ptr) override
	{
		if (ptr < data || ptr >= &data[ArenaSize])
			return;

		uint32_t size{};
		const uint8_t* size_ptr = static_cast<const uint8_t*>(ptr) - sizeof(uint32_t);
		memcpy(&size, size_ptr, sizeof(uint32_t));
		
		// Если освобождается только что выделенная память, функционал аналогичен стеку
		if (&data[_inner_idx - size] == ptr)
			_inner_idx -= size;
	}

	void Clear(const void* ptr)
	{
		if (ptr < data || ptr >= &data[ArenaSize])
			return;

		_inner_idx = (static_cast<const uint8_t*>(ptr) - data - sizeof(uint32_t));
	}

	void Clear() override
	{
		_inner_idx = 0;
	}
};


#endif
