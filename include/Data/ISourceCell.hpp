/**
 * @file ISourceCell.hpp
 * @date Oct 14, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_I_SOURCE_CELL_HPP_
#define INC_DATA_I_SOURCE_CELL_HPP_

// Интерфейс ячейки с данными от разных источников
template <typename Type, typename Source>
struct ISourceCell
{
    virtual ~ISourceCell() = default;
	virtual void Set(const Type& value, Source source) = 0;
	virtual bool Get(Type& value, Source source) const = 0;
	virtual Type GetOrDefault(Source source) const = 0;
	virtual void Clear(Source source) = 0;
};

#endif
