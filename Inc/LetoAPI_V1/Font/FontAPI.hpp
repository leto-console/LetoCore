/**
 * @file FontAPI_V1.hpp
 * @date May 30, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_FONT_FONT_API_V1_HPP_
#define INC_LETO_API_V1_FONT_FONT_API_V1_HPP_

#include <cstdint>
#include <type_traits>

#include <Graphics/IFont.hpp>

// Выравнивание по границе 4 байт 
#pragma pack(push, 4)

/// Функции для работы со шрифтами
struct FontAPI_V1
{
    /// TODO: Интерфейс шрифта нужно изменить - он C++-like с v-table
    /**
     * @brief Получить шрифт
     * 
     * @param width Ширина шрифта
     * @param height Высота шрифта
     * @param type Тип шрифта (0 - обычный, 1 - маленький)
     */
    IFont* (*GetFont)(uint32_t width, uint32_t height, uint32_t type);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<FontAPI_V1>::value, "FontAPI_V1 должен быть standard_layout");

#endif
