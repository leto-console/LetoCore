/**
 * @file BitmapData.hpp
 * @date Dec 26, 2025
 * @author Timur
 * 
 */

#ifndef INC_GRAPHICS_BITMAP_DATA_HPP_
#define INC_GRAPHICS_BITMAP_DATA_HPP_

#include "LibrariesExport.h"

#include <stdint.h>
#include <cstring>
#include <Data/IAllocator.hpp>

/**
 * @brief Флаг битмапа
 */
enum BitmapFlags
{
	BMF_RLE = (1 << 1),		///< Используется RLE для кодирования изображения
};

/**
 * @brief Битмап
 * @headerfile BitmapData.hpp <Graphics/BitmapData.hpp>
 * 
 * Используется для хранения и передачи битовых карт - битмапов
 */
struct LIBRARIES_EXPORT BitmapData
{
	uint8_t width{};	///< Ширина битмапа
	uint8_t height{};	///< Высота битмапа

	const uint8_t* bitmap{};		///< Основное изображение
	const uint8_t* background{};	///< Фоновое изображение

	uint32_t ID{};		///< Идентификатор для поиска
	uint32_t flags{};	///< Флаги битмапа

	/**
	 * @brief Инициализация фонового изображения
	 * 
	 * @warning Функция затирает существующее фоновое изображение
	 * 
	 * @param allocator Аллокатор, из которого будет запрашиваться память
	 */
	void MakeBackground(IAllocator* allocator)
	{
		background = (uint8_t*) allocator->Alloc(Size());
		memset(const_cast<uint8_t*>(background), 0, Size());
	}

	/**
	 * @brief Глубокая копия одного битмапа из другого
	 * 
	 * Запрашивает новые блоки памяти под требуемые изображение и копирует их
	 * 
	 * @param allocator Аллокатор, из которого будет запрашиваться память
	 * @param b_bitmap Нужно ли копировать основное изображение
	 * @param b_background Нужно ли копировать фоновое изображение
	 */
	void CopyFrom(IAllocator* allocator, BitmapData& from_data)
	{
		height = from_data.height;
		width = from_data.width;
		ID = from_data.ID;
		flags = from_data.flags;
		if (from_data.bitmap)
		{
			bitmap = (uint8_t*) allocator->Alloc(from_data.Size());
			memcpy(const_cast<uint8_t*>(bitmap), from_data.bitmap, Size());
		}
		if (from_data.background)
		{
			background = (uint8_t*) allocator->Alloc(from_data.Size());
			memcpy(const_cast<uint8_t*>(background), from_data.background, Size());
		}
	}

	/**
	 * @brief Глубокая копия битмапа
	 * 
	 * Запрашивает новые блоки памяти под требуемые изображение и копирует их в новый битмап
	 * 
	 * @param allocator Аллокатор, из которого будет запрашиваться память
	 * @param b_bitmap Нужно ли копировать основное изображение
	 * @param b_background Нужно ли копировать фоновое изображение
	 */
	BitmapData CopyTo(IAllocator* allocator, bool b_bitmap = true, bool b_background = false)
	{
		uint8_t *copy_bitmap{}, *copy_background{};
		if (b_bitmap && bitmap)
		{
			copy_bitmap = (uint8_t*) allocator->Alloc(Size());
			memcpy(copy_bitmap, bitmap, Size());
		}
		if (b_background && background)
		{
			copy_background = (uint8_t*) allocator->Alloc(Size());
			memcpy(copy_background, bitmap, Size());
		}
		return { width, height, copy_bitmap, copy_background, ID, flags };
	}

	/**
	 * @brief Размер битмапа в памяти
	 */
	uint32_t Size() const
	{
		return ((height + 7) >> 3) * width;
	}
};

/**
 * @brief Установить пиксель
 * 
 * @param data Указатель на битмап
 * @param width Ширина битмапа
 * @param height Высота битмапа
 * @param x,y Координаты пикселя
 */
inline LIBRARIES_EXPORT void SetPixel(uint8_t* data, uint8_t width, uint8_t height, int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return;
	}

	const int width_bytes = (height + 7) >> 3;
	const int idx = ((width_bytes * x) + (y >> 3));

	data[idx] |= 1 << (y & 0x7);
}

/**
 * @brief Получить состояние пикселя
 * 
 * @param data Указатель на битмап
 * @param width Ширина битмапа
 * @param height Высота битмапа
 * @param x,y Координаты пикселя
 * 
 * @return Состояние пикселя (1/0)
 */
inline LIBRARIES_EXPORT bool GetPixel(const uint8_t* data, uint8_t width, uint8_t height, int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return false;
	}
	
	const int width_bytes = (height + 7) >> 3;
	const int idx = ((width_bytes * x) + (y >> 3));

	return (data[idx] >> (y & 0x7)) & 1;
}

#endif