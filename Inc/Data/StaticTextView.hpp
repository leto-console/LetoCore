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

	StaticTextView(const char* text) noexcept 
		: begin_ptr{ text }, capacity{ strlen(text) } { }

	const char* ConstChar() const noexcept { return begin_ptr; }

	bool Empty() const noexcept { return TextLength() == 0; }

	// Размер строки (без нулей)
	size_t TextLength() const noexcept { return strnlen(begin_ptr, capacity); }

	// Запас по символам (без последнего нуля)
	size_t Capacity() const noexcept { return capacity; }

	const char& operator[](size_t index) const noexcept { return begin_ptr[index]; }

	// Для чтения (const-контекст): for (const char& c : myText)
	const char* begin() const noexcept { return begin_ptr; }
	const char* end() const noexcept { return begin_ptr + TextLength(); }
};

#endif