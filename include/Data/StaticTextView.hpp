/**
 * @file StaticTextView.hpp
 * @date Mar 15, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_STATIC_TEXT_VIEW_HPP_
#define INC_DATA_STATIC_TEXT_VIEW_HPP_

#include <Data/StaticText.hpp>

struct StaticTextView
{
protected:
	const char* begin_ptr;
	const size_t capacity;

public:
	template <size_t Capacity>
	constexpr StaticTextView(const StaticText<Capacity>& text) noexcept 
		: begin_ptr{ text.begin() }, capacity{ Capacity } { }

	constexpr StaticTextView(const char* text) noexcept 
		: begin_ptr{ text }, capacity{ constexpr_string::strlen(text) } { }

	constexpr const char* ConstChar() const noexcept { return begin_ptr; }

	constexpr bool Empty() const noexcept { return TextLength() == 0; }

	// Размер строки (без нулей)
	constexpr size_t TextLength() const noexcept { return constexpr_string::strnlen(begin_ptr, capacity); }

	// Запас по символам (без последнего нуля)
	constexpr size_t Capacity() const noexcept { return capacity; }

	constexpr const char& operator[](size_t index) const noexcept { return begin_ptr[index]; }

	// Для чтения (const-контекст): for (const char& c : myText)
	constexpr const char* begin() const noexcept { return begin_ptr; }
	constexpr const char* end() const noexcept { return begin_ptr + TextLength(); }
};

#endif