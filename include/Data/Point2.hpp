/**
 * @file Point.hpp
 * @date Dec 26, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_POINT_HPP_
#define INC_DATA_POINT_HPP_

#include <cstdint>

/**
 * @brief 2D точка с координатами X и Y.
 * @tparam T Тип координат (int, float, uint16_t и т.д.)
 * @note POD-тип, передаётся по значению (8 байт для int)
 */
template <typename T>
struct Point2
{
	T x{};
	T y{};

	constexpr Point2() = default;
    constexpr Point2(T x_, T y_) : x(x_), y(y_) {}

	constexpr bool operator==(const Point2& other) const 
	{
		return x == other.x && y == other.y;
	}

	constexpr bool operator!=(const Point2& other) const 
	{
		return !(*this == other);
	}

	constexpr Point2 operator+(const Point2& other) const 
	{
        return {x + other.x, y + other.y};
    }

    constexpr Point2 operator-() const
	{
        return {-x, -y};
    }

    constexpr Point2 operator-(const Point2& other) const 
	{
        return {x - other.x, y - other.y};
    }

	constexpr void operator+=(const Point2& other) 
	{
		*this = *this + other;
    }

	constexpr void operator-=(const Point2& other) 
	{
		*this = *this - other;
    }

	constexpr void operator+=(T value)
	{
		this->x += value;
		this->y += value;
	}

	constexpr void operator-=(T value)
	{
		this->x -= value;
		this->y -= value;
	}

	constexpr void operator*=(T value)
	{
		this->x *= value;
		this->y *= value;
	}

};

using Point2_i = Point2<int>;
using Point2_u = Point2<unsigned>;
using Point2_f = Point2<float>;

using Point2_u8 = Point2<uint8_t>;
using Point2_u16 = Point2<uint16_t>;
using Point2_u32 = Point2<uint32_t>;

using Point2_i8 = Point2<int8_t>;
using Point2_i16 = Point2<int16_t>;
using Point2_i32 = Point2<int32_t>;

#endif
