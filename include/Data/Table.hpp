/**
 * @file Table.hpp
 * @date Dec 13, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_TABLE_HPP_
#define INC_DATA_TABLE_HPP_

#include <cstdint>
#include <cstring>

//using T = float;

template <typename Cell, typename TAllocator>
class Table
{
protected:
	Cell* data{};
	int rows_count, cols_count;
	TAllocator* allocator{};

	bool CheckRange(int row, int col) const
	{
		return row >= 0 && row < rows_count && col >= 0 && col < cols_count;
	}

public:
	Table(int rows_count, int cols_count, TAllocator* allocator = nullptr)
		: rows_count{ rows_count }, cols_count{ cols_count }, allocator{ allocator }
	{
		if (!allocator)
			data = new Cell[rows_count * cols_count]{};
		else
		{
			void* ptr = allocator->Alloc(rows_count * cols_count * sizeof(Cell));
			data = static_cast<Cell*>(ptr);
		}
	}

	~Table()
	{
		if (!allocator && data) delete[] data;
	}

	int Rows() const { return rows_count; }
	int Cols() const { return cols_count; }

	void CopyFrom(const Table<Cell, TAllocator>& table)
	{
		if (table.rows_count != rows_count ||
			table.cols_count != cols_count)
			return;
		memcpy(data, table.data, rows_count * cols_count * sizeof(Cell));
	}

	void Clear()
	{
		memset(data, 0, rows_count * cols_count * sizeof(Cell));
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
		data[row * cols_count + col] = value;
	}

	void RawGetCell(int row, int col, Cell& value) const
	{
		value = data[row * cols_count + col];
	}
};

template <typename TAllocator>
class Table<bool, TAllocator>
{
	using Cell = bool;
protected:
	uint8_t* data{};
	int rows_count, cols_count;
	TAllocator* allocator{};

	bool CheckRange(int row, int col) const
	{
		return row >= 0 && row < rows_count && col >= 0 && col < cols_count;
	}

public:
	Table(int rows_count, int cols_count, TAllocator* allocator = nullptr)
		: rows_count{ rows_count }, cols_count{ cols_count }, allocator{ allocator }
	{
		if (!allocator)
			data = new uint8_t[rows_count * cols_count >> 3]{};
		else
		{
			void* ptr = allocator->Alloc(rows_count * cols_count >> 3);
			data = static_cast<uint8_t*>(ptr);
		}
		Clear();
	}

	~Table()
	{
		if (!allocator && data) delete[] data;
	}

	int Rows() const { return rows_count; }
	int Cols() const { return cols_count; }

	void CopyFrom(const Table<Cell, TAllocator>& table)
	{
		if (table.rows_count != rows_count ||
			table.cols_count != cols_count)
			return;
		memcpy(data, table.data, rows_count * cols_count >> 3);
	}

	void Clear()
	{
		memset(data, 0, rows_count * cols_count >> 3);
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
		int idx = row * cols_count + col;
		if (value)
			data[idx >> 3] |= (1 << (idx & 0x7));
		else
			data[idx >> 3] &= ~(1 << (idx & 0x7));
	}

	void RawGetCell(int row, int col, Cell& value) const
	{
		int idx = row * cols_count + col;
		value = (data[idx >> 3] >> (idx & 0x7)) & 1;
	}
};


#endif
