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

// Align to 4-byte boundary
#pragma pack(push, 4)

/// Mathematical functions
struct MathAPI_V1
{
    /// Arccosine function
    float (*acosf)(float x);

    /// Arcsine function
    float (*asinf)(float x);

    /// Arctangent function
    float (*atanf)(float x);
    
    /// Arctangent function with two arguments
    float (*atan2f)(float y, float x);

    /// @brief Cosine function
    float (*cosf)(float x);

    /// @brief Sine function
    float (*sinf)(float x);

    /// @brief Tangent function
    float (*tanf)(float x);

    /// @brief Round down to nearest integer
    float (*floorf)(float x);

    /// @brief Round up to nearest integer
    float (*ceilf)(float x);

    /// @brief Round to nearest integer
    float (*roundf)(float x);

    /// @brief Square root
    float (*sqrtf)(float x);

    /// @brief Power function
    float (*powf)(float x, float y);

    /// @brief Absolute value
    float (*fabs)(float x);

    /// @brief Hypotenuse calculation
    float (*hypotf)(float x, float y);

    /// @brief Exponential function
    float (*expf)(float x);

    /// @brief Natural logarithm
    float (*logf)(float x);

    /// @brief Floating-point remainder of division
    float (*fmodf)(float x, float y);

    /// @brief Sign copysign function
    float (*copysignf)(float number, float sign);

    /// @brief Check if value is NaN
    bool (*isnanf)(float x);

    /// @brief Check if value is infinity
    bool (*isinff)(float x);
};

#pragma pack(pop)

static_assert(std::is_standard_layout<MathAPI_V1>::value, "MathAPI_V1 must be a standard layout type");

#endif
