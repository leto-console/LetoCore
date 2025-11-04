/**
 * @file SupportAPI_V1.hpp
 * @date May 23, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_SUPPORT_SUPPORT_API_V1_HPP_
#define INC_LETO_API_V1_SUPPORT_SUPPORT_API_V1_HPP_

#include <cstdint>
#include <type_traits>

enum
{
	LETO_API_IMPORT_UNKNOWN = -1,
	LETO_API_IMPORT_SUCCESS = 0,
	LETO_API_IMPORT_ERROR = 1
};

#pragma pack(push, 4)

/// Контроль поддержки функций
struct SupportAPI_V1
{
    /// Импортировать функцию
    void (*Import) (const char* function);
    
    /// Проверить состояние импорта
    int (*CheckImport) ();

    /// Сбросить результат импорта
    void (*ResetCheckResult) ();
};

#pragma pack(pop)

static_assert(std::is_standard_layout<SupportAPI_V1>::value, "SupportAPI_V1 должен быть standard_layout");

#endif
