/**
 * @brief RingFIFO_Static.hpp
 * @date Mar 05, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_RING_FIFO_STATIC_HPP_
#define INC_DATA_RING_FIFO_STATIC_HPP_

#include <cstdint>

 //using T = float;

template <typename T, int BufferSize>
class RingFIFO_Static
{
protected:
	T data[BufferSize]{};
	uint32_t iRead{}, iWrite{};
	uint32_t size{ BufferSize };
	uint32_t count{};

public:
	RingFIFO_Static() = default;

	uint32_t Count() const { return count; }
	uint32_t Size() const { return size; }

	void Clear()
	{
		count = iRead = iWrite = 0;
	}

	void Push(const T& value)
	{
		data[iWrite] = value;
		++iWrite %= size;

		if (count < size) 
			++count;
		if (count == size)
			++iRead %= size;
	}

	bool Front(T& value)
	{
		if (iRead == iWrite) return false;
		value = data[iRead];
		return true;
	}

	bool Pop()
	{
		if (iRead == iWrite) return false;
		++iRead %= size;
		--count;
		return true;
	}

	bool Pop(T& value)
	{
		if (!Front(value))
			return false;
		return Pop();
	}

	T Sum() const
	{
		T sum{};
		for (const T& value : *this)
			sum += value;
		return sum;
	}

	// for-each support
	T		*begin()		{ return data; }
	T const	*begin() const	{ return data; }
	T		*end()		 { return data + count; }
	T const	*end() const { return data + count; }
};

#endif