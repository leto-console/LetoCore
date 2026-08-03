/**
 * @file StaticListView.hpp
 * @date Mar 15, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_STATIC_LIST_VIEW_HPP_
#define INC_DATA_STATIC_LIST_VIEW_HPP_

#include <Data/StaticList.hpp>
#include <Data/Range.hpp>
#include <iterator>

template <typename Type>
class StaticListView
{
private:
	const Type* begin_ptr{};
    const size_t* count{};
    size_t max_count{};

public:
	constexpr StaticListView() = default;

	template <typename T, size_t MaxCount>
	constexpr StaticListView(const StaticList<T, MaxCount>& list) noexcept
        : begin_ptr{ list.begin() }, count{ &list.count }, max_count{ MaxCount }
	{ }

	template <typename T, size_t MaxCount>
	constexpr void operator=(const StaticList<T, MaxCount>& list) noexcept
	{
		begin_ptr = list.begin();
		count = &list.count;
		max_count = MaxCount;
	}

	constexpr bool Empty() const { return *count == 0; }
	constexpr bool Full() const { return *count >= max_count; }

    constexpr size_t Count() const noexcept { return *count; }
	constexpr size_t Size() const noexcept { return max_count; }

	constexpr const Type& Back() 	const	{ return operator[](*count - 1); }

	constexpr const Type& operator[](size_t i) const noexcept { return *(begin_ptr + i); }

	constexpr const Type* begin() 	const noexcept	{ return begin_ptr; }
	constexpr const Type* end() 	const noexcept	{ return begin_ptr + *count; }

	/// Получить инвертированный `view`
    constexpr Range<std::reverse_iterator<const Type*>> reversed() const noexcept 
	{ 
		return Range<std::reverse_iterator<const Type*>>{
            std::make_reverse_iterator(end()), 
            std::make_reverse_iterator(begin())
        };
	}

	// Support for std::vector interface

    constexpr bool empty() const noexcept { return Empty(); }
    constexpr size_t size() const noexcept { return Count(); }

};

#endif
