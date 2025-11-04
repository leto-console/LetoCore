/**
 * @brief RingFIFO_Static.hpp
 * @date Mar 05, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_RING_FIFO_STATIC_HPP_
#define INC_DATA_RING_FIFO_STATIC_HPP_

#include <cstdint>

 //using T = float;

template <typename T, int Size>
class RingFIFO_Static
{
protected:
	T data[Size]{};
	uint32_t iRead{}, iWrite{};
	uint32_t size{ Size };

public:
	RingFIFO_Static()
	{
	}

	uint32_t Count() const
	{
		return (iWrite - iRead) % size;
	}

	void Push(const T& value)
	{
		data[iWrite] = value;
		++iWrite %= size;
		if (iWrite == iRead)
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
	T		*end()		 { return data + Size; }
	T const	*end() const { return data + Size; }
};

#endif