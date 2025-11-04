/**
 * @file StaticText.hpp
 * @date Dec 17, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_STATIC_TEXT_HPP_
#define INC_DATA_STATIC_TEXT_HPP_

#include <cstring>

template <size_t TextCapacity = 8>
struct StaticText
{	
protected:
	char text[TextCapacity + 1]{};

public:
	StaticText() = default;

	StaticText(const char* value)
	{
		memcpy(text, value, strnlen(value, TextCapacity));
	}

	template <size_t OtherCapacity>
	StaticText(const StaticText<OtherCapacity>& other)
	{
		memcpy(text, other.ConstChar(), OtherCapacity < TextCapacity ? OtherCapacity : TextCapacity);
	}

	operator const char* () const
	{
		return text;
	}

	void operator+= (const StaticText& other)
	{
		if (TextLength() < TextCapacity)
		{
			size_t available = TextCapacity - TextLength();
			memcpy(&text[TextLength()], other.text, other.TextLength() < available ? other.TextLength() : available);
		}
	}

	StaticText operator+ (const StaticText& other) const
	{
		StaticText left_op = *this;
		left_op += other;
		return left_op;
	}

	StaticText& operator= (const StaticText& other)
	{
		memcpy(text, other.text, TextCapacity);
		return *this;
	}

	bool operator==(const char* other) const
	{
		return strncmp(text, other, TextCapacity) == 0;
	}

	bool operator==(const StaticText& other) const
	{
		return strncmp(text, other.text, TextCapacity) == 0;
	}

	template <size_t OtherCapacity>
	bool operator==(const StaticText<OtherCapacity>& other) const
	{
		return strncmp(text, other.ConstChar(), TextCapacity) == 0;
	}

	const	char& operator[](size_t index) const	{ return text[index]; }
			char& operator[](size_t index)			{ return text[index]; }

	char*		CharPtr()			{ return text; }
	const char* ConstChar() const	{ return text; }

	bool Empty() const { return TextLength() == 0; }

	// Размер строки (без нулей)
	size_t TextLength() const { return strnlen(text, TextCapacity); }

	// Запас по символам (без последнего нуля)
	size_t Capacity() const { return TextCapacity; }

	// for-each logic:

	// Для чтения и изменения: for (char& c : myText)
	char* begin() { return text; }
	char* end() { return text + TextLength(); }

	// Для чтения (const-контекст): for (const char& c : myText)
	const char* begin() const { return text; }
	const char* end() const { return text + TextLength(); }
};

// Статический текст величиной не более 8 символов
using StaticText8 = StaticText<8>;

// Статический текст величиной не более 16 символов
using StaticText16 = StaticText<16>;

// Статический текст величиной не более 32 символов
using StaticText32 = StaticText<32>;

// Статический текст величиной не более 64 символов
using StaticText64 = StaticText<64>;

#endif