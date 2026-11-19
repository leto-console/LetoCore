/**
 * @file Rect.hpp
 * @date Dec 26, 2025
 * @author Rakhimov T.
 */

#pragma once

#include <cstdint>

#include <Data/Point2.hpp>

/**
 * @brief Прямоугольник
 * @tparam T Тип координат (int, float, uint16_t и т.д.)
 */
template <typename T>
struct Rect
{
	Point2<T> pos{};
	Point2<T> size{};

	constexpr Rect() = default;
    constexpr Rect(Point2<T> pos_, Point2<T> size_) : pos(pos_), size(size_) {}

	constexpr bool operator==(const Rect& other) const 
	{
		return pos == other.pos && size == other.size;
	}

	constexpr bool operator!=(const Rect& other) const 
	{
		return !(*this == other);
	}

};

using Rect_i = Rect<int>;
//using Rect_u = Point2<unsigned>;
using Rect_f = Rect<float>;

//using Point2_u8 = Point2<uint8_t>;
//using Point2_u16 = Point2<uint16_t>;
//using Point2_u32 = Point2<uint32_t>;

using Rect_i8 = Rect<int8_t>;
using Rect_i16 = Rect<int16_t>;
using Rect_i32 = Rect<int32_t>;
