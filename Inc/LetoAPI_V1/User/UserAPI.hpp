/**
 * @file UserAPI_V1.hpp
 * @date May 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_USER_USER_API_V1_HPP_
#define INC_LETO_API_V1_USER_USER_API_V1_HPP_

#include <cstdint>
#include <type_traits>

#include <Storage/NamedSpace.hpp>
#include <Graphics/BitmapData.hpp>

// Выравнивание по границе 4 байт 
#pragma pack(push, 4)

/// Функции для работы с пользователями
struct UserAPI_V1
{
    /// @brief Получить пользовательское пространство
    NamedSpace* (*GetUserSpace)();
    
    /// @brief Получить аватар по идентификатору
    bool (*GetAvatarByID)(uint32_t ID, BitmapData* data);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<UserAPI_V1>::value, "UserAPI_V1 должен быть standard_layout");

#endif
