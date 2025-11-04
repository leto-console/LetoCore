/*
 * BitmapEffects.hpp
 *
 *  Created on: Dec 26, 2025
 *      Author: Timur
 */

#ifndef INC_GRAPHICS_BITMAP_EFFECTS_HPP_
#define INC_GRAPHICS_BITMAP_EFFECTS_HPP_

#include "BitmapData.hpp"
#include <stdint.h>
#include <cstring>
#include <Data/IAllocator.hpp>
#include <Data/Point2.hpp>

#include <LetoAPI_V1/LetoAPI_V1.hpp>

namespace BitmapEffects
{
    inline void MakeOutlined(BitmapData& data, IAllocator* allocator, int radius = 1)
    {
        data.MakeBackground(allocator);
        
        uint8_t* background = const_cast<uint8_t*>(data.background);
        memcpy(background, data.bitmap, data.width * data.height >> 3);

        bool next = false;
        for (int _x = 0; _x < data.width; ++_x)
        {
            for (int _y = 0; _y < data.height; ++_y)
            {
                next = false;
                for (int i = -radius; i <= radius; ++i)
                {
                    for (int j = -radius; j <= radius; ++j)
                    {
                        if (i == 0 && j == 0) continue;
                        if (GetPixel(data.bitmap, data.width, data.height, _x + i, _y + j))
                        {
                            SetPixel(background, data.width, data.height, _x, _y);
                            next = true;
                            break;
                        }
                    }
                    if (next) break;
                }
            }
        }
    }

    inline void MakeSticker(BitmapData& data, IAllocator* allocator, int radius = 1)
    {
        MakeOutlined(data, allocator, radius);

        BitmapData copy = data.CopyTo(allocator, true, false);

        bool next = false;
        for (int _x = 0; _x < data.width; ++_x)
        {
            for (int _y = 0; _y < data.height; ++_y)
            {
                next = false;
                if (GetPixel(data.bitmap, data.width, data.height, _x, _y) ||
                    GetPixel(data.background, data.width, data.height, _x, _y))
                    continue;
                for (int i = -(1); i <= (1); ++i)
                {
                    for (int j = -(1); j <= (1); ++j)
                    {
                        if (GetPixel(data.background, data.width, data.height, _x + i, _y + j))
                        {
                            SetPixel(const_cast<uint8_t*>(copy.bitmap), data.width, data.height, _x, _y);
                            next = true;
                            break;
                        }
                    }
                    if (next) break;
                }
            }
        }
        memcpy(const_cast<uint8_t*>(data.bitmap), copy.bitmap, data.width * ((data.height + 7) >> 3));

        if (copy.bitmap)
            allocator->Free(copy.bitmap);
        if (copy.background)
            allocator->Free(copy.background);
    }

    /**
     * @brief Вспомогательная функция для поворота одного слоя битмапа
     */
    inline void RotateBitmapLayer(const uint8_t* src_data, uint8_t* dst_data, 
                                uint8_t width, uint8_t height,
                                float cos_a, float sin_a, float cx, float cy)
    {
        // Очищаем целевой буфер
        memset(dst_data, 0, ((height + 7) >> 3) * width);
        
        // Обратное отображение
        for (uint8_t y = 0; y < height; ++y)
        {
            for (uint8_t x = 0; x < width; ++x)
            {
                const float dx = static_cast<float>(x) - cx;
                const float dy = static_cast<float>(y) - cy;
                
                const float src_x = dx * cos_a + dy * sin_a + cx;
                const float src_y = -dx * sin_a + dy * cos_a + cy;
                
                const int ix = static_cast<int>(src_x + 0.5f);
                const int iy = static_cast<int>(src_y + 0.5f);
                
                if (ix >= 0 && ix < width && iy >= 0 && iy < height)
                {
                    if (GetPixel(src_data, width, height, ix, iy))
                    {
                        SetPixel(dst_data, width, height, x, y);
                    }
                }
            }
        }
    }

    /**
     * @brief Повернуть битмап и фон на указанный угол вокруг центра
     * 
     * @param src_bitmap Исходный битмап (не изменяется)
     * @param dst_bitmap Целевой битмап для записи результата (должен иметь те же размеры)
     * @param angle_degrees Угол поворота (положительный — против часовой стрелки)
     * 
     * @return true если поворот выполнен успешно
     * @return false если размеры не совпадают или невалидные данные
     * 
     * @note Вращает и основное изображение (bitmap), и фон (background) если они существуют
     * @note Целевые буферы очищаются перед отрисовкой (фон = 0)
     * @warning src_bitmap и dst_bitmap должны быть разными объектами!
     */
    inline bool RotateBitmap(const BitmapData& src_bitmap, BitmapData& dst_bitmap, float angle_degrees)
    {
        // 1. Проверка размеров
        if (src_bitmap.width != dst_bitmap.width || src_bitmap.height != dst_bitmap.height)
        {
            return false; // размеры не совпадают
        }
        
        // 2. Проверка на валидность данных
        if (!src_bitmap.bitmap || !dst_bitmap.bitmap)
        {
            return false; // основное изображение обязательно
        }
        
        // 3. Проверка что это не один и тот же объект
        if (&src_bitmap == &dst_bitmap || src_bitmap.bitmap == dst_bitmap.bitmap)
        {
            return false; // src и dst должны быть разными буферами!
        }
        
        // 4. Если есть фон, проверяем что он тоже существует в dst
        if (src_bitmap.background && !dst_bitmap.background)
        {
            return false; // фон есть в src, но нет в dst
        }
        
        // 5. Проверка на совпадение фоновых буферов
        if (src_bitmap.background && dst_bitmap.background && 
            src_bitmap.background == dst_bitmap.background)
        {
            return false; // фоновые буферы не должны совпадать!
        }
        
        const uint8_t width = src_bitmap.width;
        const uint8_t height = src_bitmap.height;
        
        // 6. Центр вращения и тригонометрия
        const float cx = width * 0.5f;
        const float cy = height * 0.5f;
        const float angle = angle_degrees * 0.017453292519943295f; // deg → rad
        const float cos_a = leto_api_v1->Math->cosf(angle);
        const float sin_a = leto_api_v1->Math->sinf(angle);
        
        /// TODO: нужно ли так плотно связывать работу с LetoAPI?

        // 7. Вращаем основное изображение
        RotateBitmapLayer(src_bitmap.bitmap, const_cast<uint8_t*>(dst_bitmap.bitmap),
                        width, height, cos_a, sin_a, cx, cy);
        
        // 8. Если есть фон, вращаем его тоже
        if (src_bitmap.background && dst_bitmap.background)
        {
            RotateBitmapLayer(src_bitmap.background, const_cast<uint8_t*>(dst_bitmap.background),
                            width, height, cos_a, sin_a, cx, cy);
        }
        
        return true;
    }

    /**
     * @brief Создать копию битмапа с указанным целевым размером
     * 
     * @param src Исходный битмап
     * @param allocator Аллокатор для выделения памяти
     * @param target_size Желаемый размер {ширина, высота}
     * @return BitmapData Копия с целевым размером, или пустой BitmapData при ошибке
     * 
     * @note Если target_size > src: изображение центрируется, края заполняются прозрачным (0)
     * @note Если target_size < src: изображение обрезается по центру
     * @note Если есть фон (background), он обрабатывается аналогично основному слою
     * @note Ответственность за освобождение памяти лежит на вызывающем коде
     */
    inline BitmapData ResizeCopy(const BitmapData& src, IAllocator* allocator, Point2_i target_size)
    {
        BitmapData result;
        
        // 1. Валидация
        if (!src.bitmap || !allocator || target_size.x <= 0 || target_size.y <= 0)
        {
            return result;
        }
        
        // Проверка на допустимый диапазон (если BitmapData использует uint8_t для размеров)
        if (target_size.x > 255 || target_size.y > 255)
        {
            return result;
        }
        
        const uint8_t new_width  = static_cast<uint8_t>(target_size.x);
        const uint8_t new_height = static_cast<uint8_t>(target_size.y);
        const size_t data_size = ((new_height + 7) >> 3) * new_width;
        
        // 2. Выделяем основной буфер
        result.bitmap = static_cast<uint8_t*>(allocator->Alloc(data_size));
        if (!result.bitmap) return BitmapData();
        memset(const_cast<uint8_t*>(result.bitmap), 0, data_size); // прозрачный фон по умолчанию
        
        // 3. Выделяем фон если нужно
        if (src.background)
        {
            result.background = static_cast<uint8_t*>(allocator->Alloc(data_size));
            if (!result.background)
            {
                allocator->Free(result.bitmap);
                result.bitmap = nullptr;
                return BitmapData();
            }
            memset(const_cast<uint8_t*>(result.background), 0, data_size);
        }
        
        result.width  = new_width;
        result.height = new_height;
        
        // 4. Вычисляем области копирования (пересечение исходного и целевого)
        // Смещение исходного изображения относительно нового канваса
        const int src_offset_x = (target_size.x - src.width) / 2;   // может быть отрицательным при обрезке
        const int src_offset_y = (target_size.y - src.height) / 2;
        
        // Границы области, которую нужно скопировать из src
        const int copy_start_x = (src_offset_x < 0) ? -src_offset_x : 0;  // если обрезка слева
        const int copy_start_y = (src_offset_y < 0) ? -src_offset_y : 0;
        const int copy_end_x   = (src_offset_x > 0) ? src.width : src.width - src_offset_x;
        const int copy_end_y   = (src_offset_y > 0) ? src.height : src.height - src_offset_y;
        
        // Границы области, куда копировать в dst
        const int dst_start_x = (src_offset_x > 0) ? src_offset_x : 0;
        const int dst_start_y = (src_offset_y > 0) ? src_offset_y : 0;
        
        // 5. Копируем основное изображение
        for (int sy = copy_start_y; sy < copy_end_y; ++sy)
        {
            for (int sx = copy_start_x; sx < copy_end_x; ++sx)
            {
                const int dx = dst_start_x + (sx - copy_start_x);
                const int dy = dst_start_y + (sy - copy_start_y);
                
                if (GetPixel(src.bitmap, src.width, src.height, static_cast<uint8_t>(sx), static_cast<uint8_t>(sy)))
                {
                    SetPixel(const_cast<uint8_t*>(result.bitmap), new_width, new_height, static_cast<uint8_t>(dx), static_cast<uint8_t>(dy));
                }
            }
        }
        
        // 6. Копируем фон если есть
        if (src.background && result.background)
        {
            for (int sy = copy_start_y; sy < copy_end_y; ++sy)
            {
                for (int sx = copy_start_x; sx < copy_end_x; ++sx)
                {
                    const int dx = dst_start_x + (sx - copy_start_x);
                    const int dy = dst_start_y + (sy - copy_start_y);
                    
                    if (GetPixel(src.background, src.width, src.height, static_cast<uint8_t>(sx), static_cast<uint8_t>(sy)))
                    {
                        SetPixel(const_cast<uint8_t*>(result.background), new_width, new_height, static_cast<uint8_t>(dx), static_cast<uint8_t>(dy));
                    }
                }
            }
        }
        
        return result;
    }
}

#endif