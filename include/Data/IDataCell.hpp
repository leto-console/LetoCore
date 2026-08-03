/**
 * @file IDataCell.hpp
 * @date Dec 19, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_I_DATA_CELL_HPP_
#define INC_DATA_I_DATA_CELL_HPP_

// Интерфейс ячейки с данными
template <typename T>
class IDataCell
{
public:
	virtual ~IDataCell() = default;
	virtual void Set(const T& value) = 0;
	virtual bool Get(T& value) const = 0;
	virtual T GetOrDefault() const = 0;
};

#endif