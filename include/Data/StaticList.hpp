/**
 * @file StaticList.hpp
 * @date Mar 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_STATIC_LIST_HPP_
#define INC_DATA_STATIC_LIST_HPP_

#include <cstring>
#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <Data/Range.hpp>

template <typename Type, size_t MaxCount>
class StaticList
{
	template <typename T>
	friend class StaticListView;
protected:
	Type data[MaxCount];
	size_t count{};

public:
	StaticList() = default;

	StaticList(std::initializer_list<Type> init)
	{
		for (const Type* it = init.begin(); it != init.end(); ++it)
			Push(*it);
	}

	constexpr bool Empty() const { return count == 0; }
	constexpr bool Full() const { return count >= MaxCount; }

	// Использование копии списка возможно только через View (для копии используется явно описанная функция CopyFrom)
	StaticList(const StaticList& list) 				= delete;
	void operator=(const StaticList& list) const 	= delete;

	bool Contains(const Type& value)
	{
		for (const Type& item : *this)
		{
			if (value == item) return true;
		}
		return false;
	}

	void Push(const Type& value)
	{
		if (Full()) return;
		memcpy(&data[count], &value, sizeof(Type));
		count++;
	}

	void Insert(Type* iter, const Type& value)
	{
		if (iter == end())
		{
			Push(value);
			return;
		}
		memmove(iter + 1, iter, ((data + MaxCount) - iter - 1) * sizeof(Type));
		*iter = value;
		if (count < MaxCount)
			count++;
	}

	void Pop()
	{
		if (count > 0)
			--count;
	}

	template <typename Predicate>
	void RemoveIf(Predicate predicate)
	{
		// 1. Проверяем возвращаемый тип (например, должен быть double)
		typedef typename std::result_of<Predicate(Type&)>::type ReturnType;

		static_assert(std::is_same<ReturnType, bool>::value, "Лямбда должна возвращать bool");

		if (count == 0)
			return;

		for (Type* obj = &Back(); obj >= begin(); --obj)
		{
			if (predicate(*obj))
				Erase(obj);
		}
	}

	void Erase(Type* pos)
	{
		if (pos != &Back())
			memmove(pos, pos + 1, (&Back() - pos) * sizeof(Type));
		memset(&Back(), 0, sizeof(Type));
		--count;
	}

	Type&		Front()			{ return data[0]; }
	const Type& Front() const	{ return data[0]; }

	Type&		Back()			{ return data[count - 1]; }
	const Type& Back() const	{ return data[count - 1]; }

	void Clear() { count = 0; }

	size_t Count() const { return count; }
	size_t Size() const { return MaxCount; }

	Type&		operator[](size_t i)		{ return data[i]; }
	const Type& operator[](size_t i) const { return data[i]; }

	// for-each support
	Type*		begin()			{ return data; }
	const Type* begin() const	{ return data; }
	Type*		end()			{ return data + count; }
	const Type* end() const		{ return data + count; }

	constexpr Range<std::reverse_iterator<const Type*>> reversed() const noexcept 
	{ 
		return Range<std::reverse_iterator<const Type*>>{
            std::make_reverse_iterator(end()), 
            std::make_reverse_iterator(begin())
        };
	}

	constexpr Range<std::reverse_iterator<Type*>> reversed() noexcept 
	{ 
		return Range<std::reverse_iterator<Type*>>{
            std::make_reverse_iterator(end()), 
            std::make_reverse_iterator(begin())
        };
	}

	// Support for std::vector interface

	void push_back(const Type& value) { Push(value); }
	void erase(Type* pos) { Erase(pos); } 
	constexpr void clear() { Clear(); }
	constexpr bool empty() const noexcept { return Empty(); }
	constexpr size_t size() const noexcept { return Count(); }

};

#endif
