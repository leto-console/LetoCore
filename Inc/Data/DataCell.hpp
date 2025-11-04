/**
 * @file DataCell.hpp
 * @date Dec 19, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_DATA_CELL_HPP_
#define INC_DATA_DATA_CELL_HPP_

#include <Data/IDataCell.hpp>

// Наследник ячейки с данными в оперативной памяти
template <typename T>
class DataCell : public IDataCell<T>
{
protected:
	T value{};
	bool inited{ false };

public:
	DataCell() = default;
	DataCell(T init_value)
	{
		Set(init_value);
	}

	void Set(const T& value) override
	{
		inited = true;
		this->value = value;
	}

	bool Get(T& value) const override
	{
		if (!inited)
			return false;
		value = this->value;
		return true;
	}

	T GetOrDefault() const override
	{
		T value{};
		if (!inited)
			return value;
		return this->value;
	}
};

#endif