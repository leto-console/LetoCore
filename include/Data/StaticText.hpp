/**
 * @file StaticText.hpp
 * @date Dec 17, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_STATIC_TEXT_HPP_
#define INC_DATA_STATIC_TEXT_HPP_

#include <cstdint>

#include <Utils/constexpr_string.hpp>

template <size_t TextCapacity = 8>
struct StaticText
{	
protected:
	char text[TextCapacity]{};
	static constexpr uint32_t MaxCapacity = (TextCapacity - 1);

public:
	constexpr StaticText() = default;

	constexpr StaticText(const char* value)
	{
		constexpr_string::memcpy(text, value, constexpr_string::strnlen(value, MaxCapacity));
	}

	template <size_t OtherCapacity>
	constexpr StaticText(const StaticText<OtherCapacity>& other)
	{
		constexpr_string::memcpy(text, other.ConstChar(), OtherCapacity < MaxCapacity ? OtherCapacity : MaxCapacity);
	}

	constexpr operator const char* () const
	{
		return text;
	}

	constexpr void operator+= (const StaticText& other)
	{
		if (TextLength() < MaxCapacity)
		{
			size_t available = MaxCapacity - TextLength();
			constexpr_string::memcpy(&text[TextLength()], other.text, other.TextLength() < available ? other.TextLength() : available);
		}
	}

	constexpr StaticText operator+ (const StaticText& other) const
	{
		StaticText left_op = *this;
		left_op += other;
		return left_op;
	}

	constexpr StaticText& operator= (const StaticText& other)
	{
		constexpr_string::memcpy(text, other.text, MaxCapacity);
		return *this;
	}

	constexpr bool operator==(const char* other) const
	{
		return constexpr_string::strncmp(text, other, MaxCapacity) == 0;
	}

	constexpr bool operator==(const StaticText& other) const
	{
		return constexpr_string::strncmp(text, other.text, MaxCapacity) == 0;
	}

	template <size_t OtherCapacity>
	constexpr bool operator==(const StaticText<OtherCapacity>& other) const
	{
		return constexpr_string::strncmp(text, other.ConstChar(), MaxCapacity) == 0;
	}

	constexpr const	char& operator[](size_t index) const	{ return text[index]; }
	constexpr		char& operator[](size_t index)			{ return text[index]; }

	constexpr char*		CharPtr()			{ return text; }
	constexpr const char* ConstChar() const	{ return text; }

	constexpr bool Empty() const { return TextLength() == 0; }

	// Размер строки (без нулей)
	constexpr size_t TextLength() const { return constexpr_string::strnlen(text, MaxCapacity); }

	// Запас по символам (без последнего нуля)
	constexpr size_t Capacity() const { return MaxCapacity; }

	// for-each logic:

	// Для чтения и изменения: for (char& c : myText)
	constexpr char* begin() { return text; }
	constexpr char* end() { return text + TextLength(); }

	// Для чтения (const-контекст): for (const char& c : myText)
	constexpr const char* begin() const { return text; }
	constexpr const char* end() const { return text + TextLength(); }
};

// Статический текст величиной не более 8 символов (с NULL-терминалом)
using StaticText8 = StaticText<8>;

// Статический текст величиной не более 16 символов (с NULL-терминалом)
using StaticText16 = StaticText<16>;

// Статический текст величиной не более 32 символов (с NULL-терминалом)
using StaticText32 = StaticText<32>;

// Статический текст величиной не более 64 символов (с NULL-терминалом)
using StaticText64 = StaticText<64>;

#include <Data/to_text.hpp>

#endif