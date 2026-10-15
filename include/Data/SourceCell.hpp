/**
 * @file SourceCell.hpp
 * @date Oct 14, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_SOURCE_CELL_HPP_
#define INC_DATA_SOURCE_CELL_HPP_

#include <Data/ISourceCell.hpp>
#include <Data/StaticMap.hpp>

// Наследник ячейки с данными от разных источников в оперативной памяти
template <typename Type, typename Source, uint32_t _Capacity>
class SourceCell : public ISourceCell<Type, Source>
{
public:
    using MapItem = typename StaticMap<Source, Type, _Capacity>::MapItem;

	SourceCell() = default;

	void Set(const Type& value, Source source = {}) override
	{
		map[source] = value;
	}

	bool Get(Type& value, Source source = {}) const override
	{
		if (!map.ContainsKey(source)) return false;
        value = map[source];
        return true;
    }

	Type GetOrDefault(Source source = {}) const override
	{
		Type value{};
		if (!map.ContainsKey(source)) return value;
		return map[source];
	}

    // for-each support
	MapItem*	    begin()			{ return map.begin(); }
	const MapItem*  begin() const	{ return map.begin(); }
	MapItem*        end()			{ return map.end(); }
	const MapItem*  end() const		{ return map.end(); }

protected:
    StaticMap<Source, Type, _Capacity> map;
};

#endif