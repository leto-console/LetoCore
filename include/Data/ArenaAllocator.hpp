/**
 * @file ArenaAllocator.hpp
 * @date Mar 05, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_ARENA_ALLOCATOR_HPP_
#define INC_DATA_ARENA_ALLOCATOR_HPP_

#include <stdint.h>
#include <cstddef>
#include <cstring>

#include <Data/IAllocator.hpp>

template <int ArenaSize, bool FreeAllowed = true>
class ArenaAllocator : public IAllocator
{
protected:
	/// Заголовок аллокации, лежит вплотную перед полезной нагрузкой
	struct Head
	{
		uint32_t prev;	///< Смещение от начала арены до начала этой аллокации
		uint32_t size;	///< Размер полезной нагрузки
	};

	static constexpr uint32_t Alignment = alignof(std::max_align_t);

	static constexpr uint32_t AlignUp(uint32_t value)
	{
		return (value + Alignment - 1) / Alignment * Alignment;
	}

	/*
		Выравнивание полезной нагрузки держится на выравнивании самой арены:
		смещение payload кратно Alignment, значит и адрес тоже. Типы с alignof
		больше Alignment этой гарантией не покрыты: на платформах, где
		max_align_t == 8 (MSVC, ARM), это касается alignas(16)
	*/
	alignas(std::max_align_t) uint8_t data[ArenaSize];
	uint32_t _inner_idx{};

	/**
	 * @brief Прочитать заголовок аллокации по указателю на её полезную нагрузку
	 * @return false, если освобождение запрещено или указатель не из арены
	 */
	bool ReadHead(const void* ptr, Head& head) const
	{
		if (!FreeAllowed || !ptr)
			return false;

		const uintptr_t base = reinterpret_cast<uintptr_t>(data);
		const uintptr_t p = reinterpret_cast<uintptr_t>(ptr);

		// Полезная нагрузка всегда идёт после заголовка и внутри арены
		if (p < base + sizeof(Head) || p >= base + static_cast<uintptr_t>(ArenaSize))
			return false;

		memcpy(&head, reinterpret_cast<const void*>(p - sizeof(Head)), sizeof(head));
		return true;
	}

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
		const uint32_t payload = AlignUp(_inner_idx + static_cast<uint32_t>(sizeof(Head)));

		/*
			Не скупись, выдели больше памяти
		*/
		if (payload >= static_cast<uint32_t>(ArenaSize) ||
			size > static_cast<uint32_t>(ArenaSize) - payload)
		{
#ifdef WIN32
			throw "Bad_alloc";
#else
			return nullptr;
#endif
		}

		const Head head{ _inner_idx, size };
		memcpy(&data[payload - sizeof(Head)], &head, sizeof(head));

		_inner_idx = payload + size;
		return &data[payload];
	}

	void Free(const void* ptr) override
	{
		Head head{};
		if (!ReadHead(ptr, head))
			return;

		// Если освобождается только что выделенная память, функционал аналогичен стеку
		const uintptr_t base = reinterpret_cast<uintptr_t>(data);
		const uintptr_t end = reinterpret_cast<uintptr_t>(ptr) + head.size;
		if (end == base + _inner_idx)
			_inner_idx = head.prev;
	}

	void Clear(const void* ptr) override
	{
		Head head{};
		if (!ReadHead(ptr, head))
			return;

		// Заголовок помнит начало своей аллокации, поэтому откат корректен
		// для любой аллокации, а не только для верхней
		_inner_idx = head.prev;
	}

	void Clear() override
	{
		if (!FreeAllowed) return;
		_inner_idx = 0;
	}
};


#endif
