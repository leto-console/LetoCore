/**
 * @file MathAPI_V1.hpp
 * @date May 30, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_MATH_MATH_API_V1_HPP_
#define INC_LETO_API_V1_MATH_MATH_API_V1_HPP_

#include <cstdint>
#include <type_traits>

#include <Graphics/IFont.hpp>

// Выравнивание по границе 4 байт 
#pragma pack(push, 4)

/// Функции для работы с математикой
struct MathAPI_V1
{
    /// Функция вычисления арккосинуса
    float (*acosf)(float x);

    /// Функция вычисления арксинуса
    float (*asinf)(float x);

    /// Вычисление арктангенса
    float (*atanf)(float x);
    
    /// Вычисление арктангенса2
    float (*atan2f)(float y, float x);

    ///@brief Вычисление косинуса
    float (*cosf)(float x);

    /// @brief Вычисление синуса
    float (*sinf)(float x);

    /// @brief Вычисление тангенса
    float (*tanf)(float x);

    /// @brief Округление вниз
    float (*floorf)(float x);

    /// @brief Округление вверх
    float (*ceilf)(float x);

    /// @brief Округление к ближайшему целому
    float (*roundf)(float x);

    /// @brief Квадратный корень
    float (*sqrtf)(float x);

    /// @brief Возведение в степень
    float (*powf)(float x, float y);

    /// @brief Абсолютное значение
    float (*fabs)(float x);

    /// @brief Вычисление гипотенузы
    float (*hypotf)(float x, float y);

    /// @brief Экспонента
    float (*expf)(float x);

    /// @brief Натуральный логарифм
    float (*logf)(float x);

    /// @brief Остаток от деления
    float (*fmodf)(float x, float y);

    /// @brief Копирование знака
    float (*copysignf)(float number, float sign);

    /// @brief Проверка на корректность
    bool (*isnanf)(float x);

    /// @brief Проверка на корректность
    bool (*isinff)(float x);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<MathAPI_V1>::value, "MathAPI_V1 должен быть standard_layout");

#endif
