#include "Make.hpp"

#include <cstdio>
#include <cstdarg>
#include <cmath>

// ====================================================================================================

#include <LetoAPI_V1/LetoAPI_V1.h>
#include <Graphics/BitmapData.hpp>

static LetoBitmap_V1* CreateBitmap(const LetoAllocator_V1* allocator, uint16_t width, uint16_t height)
{    
    if (!allocator || !width || !height) return nullptr;

    BitmapData* data = new(allocator) BitmapData{width, height, (uint8_t*) nullptr};
    data->bitmap = (uint8_t*) allocator->Alloc(BitmapData::Size(width, height));
    return BitmapData::ToHandle(data);
}

static LetoBitmap_V1* LoadFromFile(const LetoAllocator_V1* allocator, const char* path)
{
    // TODO !!!
    return nullptr;
}

static LetoBitmap_V1* CopyBitmap(const LetoBitmap_V1* source, const LetoAllocator_V1* allocator)
{
    if (source && allocator)
    {
        const BitmapData* data = BitmapData::FromHandle(source);
        if (data->bitmap)
        {
            BitmapData* copy_bitmap = BitmapData::FromHandle(CreateBitmap(allocator, data->width, data->height));
            memcpy(copy_bitmap->bitmap, data->bitmap, data->Size());
            return BitmapData::ToHandle(copy_bitmap);
        }
    }

    return nullptr;
}

static uint32_t GetWidth(const LetoBitmap_V1* bitmap)
{
    if (!bitmap) return 0;
    return BitmapData::FromHandle(bitmap)->height;
}

static uint32_t GetHeight(const LetoBitmap_V1* bitmap)
{
    if (!bitmap) return 0;
    return BitmapData::FromHandle(bitmap)->width;
}

static void SetPixel(LetoBitmap_V1* bitmap, uint32_t x, uint32_t y, bool state)
{
    if (!bitmap) return;

    BitmapData& data = *BitmapData::FromHandle(bitmap);
    data.SetPixel(x, y, state);
}

static bool GetPixel(const LetoBitmap_V1* bitmap, uint32_t x, uint32_t y)
{
    if (!bitmap) return false;

    const BitmapData& data = *BitmapData::FromHandle(bitmap);
    return data.GetPixel(x, y);
}

static LetoBitmap_V1* ResizeCopyBitmap(
    const LetoBitmap_V1* bitmap, const LetoAllocator_V1* allocator, 
    uint32_t target_width, uint32_t target_height)
{
    const BitmapData& src = *BitmapData::FromHandle(bitmap);
    
    // 1. Валидация данных
    if (!src.bitmap || !allocator || target_width <= 0 || target_height <= 0)
        return nullptr;
    
    if (target_width > 255 || target_height > 255)
        return nullptr;
    
    const uint8_t new_width  = static_cast<uint8_t>(target_width);
    const uint8_t new_height = static_cast<uint8_t>(target_height);
    const uint32_t data_size = ((new_height + 7) >> 3) * new_width;
    
    // Выделяем основной буфер
    uint8_t* new_bitmap_ptr = static_cast<uint8_t*>(allocator->Alloc(data_size));
    if (!new_bitmap_ptr) return nullptr;
    memset(const_cast<uint8_t*>(new_bitmap_ptr), 0, data_size); // прозрачный фон по умолчанию
    
    BitmapData result{new_width, new_height, new_bitmap_ptr};
    
    // 4. Вычисляем области копирования (пересечение исходного и целевого)
    // Смещение исходного изображения относительно нового канваса
    const int src_offset_x = (target_width - src.width) / 2;
    const int src_offset_y = (target_height - src.height) / 2;
    
    // Границы области, которую нужно скопировать из src
    const int copy_start_x = (src_offset_x < 0) ? -src_offset_x : 0;
    const int copy_start_y = (src_offset_y < 0) ? -src_offset_y : 0;
    const int copy_end_x   = (src_offset_x > 0) ? src.width : src.width - src_offset_x;
    const int copy_end_y   = (src_offset_y > 0) ? src.height : src.height - src_offset_y;
    
    // Границы области, куда копировать в dst
    const int dst_start_x = (src_offset_x > 0) ? src_offset_x : 0;
    const int dst_start_y = (src_offset_y > 0) ? src_offset_y : 0;
    
    // Копируем основное изображение
    for (int sy = copy_start_y; sy < copy_end_y; ++sy)
    {
        for (int sx = copy_start_x; sx < copy_end_x; ++sx)
        {
            const int dx = dst_start_x + (sx - copy_start_x);
            const int dy = dst_start_y + (sy - copy_start_y);
            
            if (src.GetPixel(sx, sy))
            {
                result.SetPixel(dx, dy, true);
            }
        }
    }
    
    return BitmapData::ToHandle(new(allocator) BitmapData(result));
}

/**
 * @brief Вспомогательная функция для поворота одного слоя битмапа
 */
static void RotateBitmapLayer(const BitmapData& src_data, BitmapData& dst_data, 
                            uint8_t width, uint8_t height,
                            float cos_a, float sin_a, float cx, float cy)
{
    // Очищаем целевой буфер
    memset(dst_data.bitmap, 0, ((height + 7) >> 3) * width);
    
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
                if (src_data.GetPixel(ix, iy))
                {
                    dst_data.SetPixel(x, y, true);
                }
            }
        }
    }
}

static bool RotateBitmap(const LetoBitmap_V1* source, LetoBitmap_V1* destination, int16_t angle_degrees)
{
    const BitmapData& src_bitmap = *BitmapData::FromHandle(source);
    BitmapData& dst_bitmap = *BitmapData::FromHandle(destination);

    // Валидация данных
    if (src_bitmap.width != dst_bitmap.width || src_bitmap.height != dst_bitmap.height)
        return false;
    
    if (!src_bitmap.bitmap || !dst_bitmap.bitmap)
        return false;
    
    // Проверка что это не один и тот же объект
    if (&src_bitmap == &dst_bitmap || src_bitmap.bitmap == dst_bitmap.bitmap)
        return false;
    
    const uint8_t width = src_bitmap.width;
    const uint8_t height = src_bitmap.height;
    
    // Центр вращения и тригонометрия
    const float cx = width * 0.5f;
    const float cy = height * 0.5f;
    const float angle = angle_degrees * 0.017453292519943295f; // deg → rad
    const float cos_a = cosf(angle);
    const float sin_a = sinf(angle);
    
    // Вращаем основное изображение
    RotateBitmapLayer(
        src_bitmap, dst_bitmap,
        width, height, cos_a, sin_a, cx, cy);
    
    return true;
}

static LetoBitmap_V1* MakeOutlinedBitmap(const LetoBitmap_V1* bitmap, const LetoAllocator_V1* allocator, int16_t radius)
{
    const BitmapData& data = *BitmapData::FromHandle(bitmap);

    BitmapData* back_data = BitmapData::FromHandle(CopyBitmap(bitmap, allocator));

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
                    if (data.GetPixel(_x + i, _y + j))
                    {
                        back_data->SetPixel(_x, _y, true);
                        next = true;
                        break;
                    }
                }
                if (next) break;
            }
        }
    }

    return BitmapData::ToHandle(back_data);
}

static LetoBitmap_V1* MakeStickerBitmap(LetoBitmap_V1* bitmap, const LetoAllocator_V1* allocator, int16_t radius)
{
    const BitmapData& data = *BitmapData::FromHandle(bitmap);
    BitmapData& back = *BitmapData::FromHandle(MakeOutlinedBitmap(bitmap, allocator, radius));

    BitmapData& copy = *BitmapData::FromHandle(CopyBitmap(bitmap, allocator));

    bool next = false;
    for (int _x = 0; _x < data.width; ++_x)
    {
        for (int _y = 0; _y < data.height; ++_y)
        {
            next = false;
            if (data.GetPixel(_x, _y) || back.GetPixel(_x, _y))
                continue;
            for (int i = -(1); i <= (1); ++i)
            {
                for (int j = -(1); j <= (1); ++j)
                {
                    if (back.GetPixel(_x + i, _y + j))
                    {
                        copy.SetPixel(_x, _y, true);
                        next = true;
                        break;
                    }
                }
                if (next) break;
            }
        }
    }
    memcpy(const_cast<uint8_t*>(data.bitmap), copy.bitmap, data.Size());

    allocator->Free(copy.bitmap);
    allocator->Free(&copy);
    return BitmapData::ToHandle(&back);
}

// ====================================================================================================

const BitmapAPI_V1* Make_BitmapAPI()
{
    static const BitmapAPI_V1 api
    {
        &CreateBitmap,
        &LoadFromFile,
        &CopyBitmap,
        &GetWidth,
        &GetHeight,
        &SetPixel,
        &GetPixel,
        &ResizeCopyBitmap,
        &RotateBitmap,
        &MakeOutlinedBitmap,
        &MakeStickerBitmap
    };
    
    return &api;
}
