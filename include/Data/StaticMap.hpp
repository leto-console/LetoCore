/**
 * @file StaticMap.hpp
 * @date Oct 14, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_STATIC_MAP_HPP_
#define INC_DATA_STATIC_MAP_HPP_

#include <cstring>
#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <Data/Range.hpp>

/**
 * @brief Статический контейнер для доступа к объектам по ключу
 */
template <typename KeyType, typename ValueType, size_t _Capacity>
class StaticMap
{
public:
	struct MapItem
	{
		KeyType 	key;
		ValueType 	value;
	};

protected:
	ValueType dummy;
	
	MapItem items[_Capacity];
	size_t size{};

public:
	StaticMap() = default;
	
	bool ContainsKey(KeyType key) const
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (items[i].key == key) 
				return true;
		}
		return false;
	}
	
	/// Если не найден ключ, возвращается ссылка на "отбойную" ячейку
	ValueType& Find(KeyType key)
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (items[i].key == key) 
				return items[i].value;
		}
		return dummy;
	}
	
	/// Если не найден ключ, возвращается ссылка на "отбойную" ячейку
	const ValueType& Find(KeyType key) const
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (items[i].key == key) 
				return items[i].value;
		}
		return dummy;
	}

	/// Если не найден ключ и кончилось место, возвращается ссылка на "отбойную" ячейку
	ValueType& FindOrCreate(KeyType key)
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (items[i].key == key) 
				return items[i].value;
		}
		if (size < _Capacity)
		{
			items[size] = MapItem{key};
			MapItem& item = items[size];
			size++;
			return item.value;
		}
		return dummy;
	}

	ValueType& 			operator[](KeyType key) 		{ return FindOrCreate(key); }
	ValueType const& 	operator[](KeyType key) const 	{ return Find(key); }


	// TODO::: добавить поддержку const auto& [key, value] ...

    // for-each support
	MapItem*	    begin()			{ return items; }
	const MapItem*  begin() const	{ return items; }
	MapItem*        end()			{ return items + size; }
	const MapItem*  end() const		{ return items + size; }

	constexpr Range<std::reverse_iterator<const MapItem*>> reversed() const noexcept 
	{ 
		return Range<std::reverse_iterator<const MapItem*>>{
            std::make_reverse_iterator(end()), 
            std::make_reverse_iterator(begin())
        };
	}

	constexpr Range<std::reverse_iterator<MapItem*>> reversed() noexcept 
	{ 
		return Range<std::reverse_iterator<MapItem*>>{
            std::make_reverse_iterator(end()), 
            std::make_reverse_iterator(begin())
        };
	}
};

#endif
