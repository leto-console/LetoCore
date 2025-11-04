/**
 * @file LetoAPI_V1.hpp
 * @date May 22, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LETO_API_V1_HPP_
#define INC_LETO_API_V1_LETO_API_V1_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <type_traits>

#include <LetoAPI_V1/Support/SupportAPI.hpp>
#include <LetoAPI_V1/Text/TextAPI.hpp>
#include <LetoAPI_V1/Globals/GlobalsAPI.hpp>
#include <LetoAPI_V1/Web/WebAPI.hpp>
#include <LetoAPI_V1/User/UserAPI.hpp>
#include <LetoAPI_V1/Font/FontAPI.hpp>
#include <LetoAPI_V1/Math/MathAPI.hpp>
#include <LetoAPI_V1/Lobby/LobbyAPI.hpp>

#define LETO_API_V1_MINOR 0

// Выравнивание по границе 4 байт 
#pragma pack(push, 4)

struct LetoAPI_V1
{
    /// Версия используемого API
    const struct VersionAPI
    {
        uint16_t major;		///< Мажорная версия используемого API
        uint16_t minor;		///< Минорная версия используемого API
    }
    *Version;

    /// Контроль поддержки функций
    const SupportAPI_V1 *Support;

    /// Функции для работы с текстом
    const TextAPI_V1 *Text;
    
    /// Функции для работы с глобальными контекстом
    const GlobalsAPI_V1* Globals;

    /// Функции для сетевого взаимодействия
    const WebAPI_V1* Web;

    /// Функции для работы с пользователями
    const UserAPI_V1* User;

    /// Функции для работы со шрифтами
    const FontAPI_V1* Font;

    /// Функции для работы с математикой
    const MathAPI_V1* Math;

    /// Функции для создания игровых комнат
    const LobbyAPI_V1* Lobby;
};

#pragma pack(pop)

/// @brief Объект LetoAPI_V1
extern LIBRARIES_EXPORT const LetoAPI_V1* leto_api_v1;

static_assert(std::is_standard_layout<LetoAPI_V1>::value, "LetoAPI_V1 должен быть standard_layout");

#endif
