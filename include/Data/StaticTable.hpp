/**
 * @file StaticTable.hpp
 * @date May 25, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_STATIC_TABLE_HPP_
#define INC_DATA_STATIC_TABLE_HPP_

#include <cstdint>
#include <cstring>

template <typename Cell, size_t RowsCount, size_t ColsCount>
class StaticTable
{
protected:
	Cell data[RowsCount * ColsCount]{};

	bool CheckRange(int row, int col) const
	{
		return row >= 0 && row < RowsCount && col >= 0 && col < ColsCount;
	}

public:
	StaticTable() = default;

	int Rows() const { return RowsCount; }
	int Cols() const { return ColsCount; }

	void CopyFrom(const StaticTable<Cell, RowsCount, ColsCount>& statictable)
	{
		memcpy(data, statictable.data, RowsCount * ColsCount * sizeof(Cell));
	}

	void Clear()
	{
		memset(data, 0, RowsCount * ColsCount * sizeof(Cell));
	}

	void SetCell(int row, int col, const Cell& value)
	{
		if (!CheckRange(row, col))
			return;
		RawSetCell(row, col, value);
	}

	bool GetCell(int row, int col, Cell& value) const
	{
		if (!CheckRange(row, col))
			return false;
		RawGetCell(row, col, value);
		return true;
	}

	void RawSetCell(int row, int col, const Cell& value)
	{
		data[row * ColsCount + col] = value;
	}

	void RawGetCell(int row, int col, Cell& value) const
	{
		value = data[row * ColsCount + col];
	}
};

template <size_t RowsCount, size_t ColsCount>
class StaticTable<bool, RowsCount, ColsCount>
{
	using Cell = bool;
protected:
	uint8_t data[RowsCount * ColsCount]{};

	bool CheckRange(int row, int col) const
	{
		return row >= 0 && row < RowsCount && col >= 0 && col < ColsCount;
	}

public:
	StaticTable() = default;

	int Rows() const { return RowsCount; }
	int Cols() const { return ColsCount; }

	void CopyFrom(const StaticTable<Cell, RowsCount, ColsCount>& statictable)
	{
		memcpy(data, statictable.data, RowsCount * ColsCount >> 3);
	}

	void Clear()
	{
		memset(data, 0, RowsCount * ColsCount >> 3);
	}

	void SetCell(int row, int col, const Cell& value)
	{
		if (!CheckRange(row, col))
			return;
		RawSetCell(row, col, value);
	}

	bool GetCell(int row, int col, Cell& value) const
	{
		if (!CheckRange(row, col))
			return false;
		RawGetCell(row, col, value);
		return true;
	}

	void RawSetCell(int row, int col, const Cell& value)
	{
		int idx = row * ColsCount + col;
		if (value)
			data[idx >> 3] |= (1 << (idx & 0x7));
		else
			data[idx >> 3] &= ~(1 << (idx & 0x7));
	}

	void RawGetCell(int row, int col, Cell& value) const
	{
		int idx = row * ColsCount + col;
		value = (data[idx >> 3] >> (idx & 0x7)) & 1;
	}
};


#endif
