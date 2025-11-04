/**
 * @file RingFIFO.hpp
 * @date Nov 04, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_RING_FIFO_HPP_
#define INC_DATA_RING_FIFO_HPP_

#include <stdint.h>

//using T = float;

template <typename T>
class RingFIFO
{
protected:
	T* data{};
	uint32_t iRead{}, iWrite{};
	uint32_t size{};

public:
	RingFIFO(uint32_t size) 
		: size{ size }
	{
		data = new T[size];
	}

	~RingFIFO()
	{
		if (data) delete[] data;
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
	T		*end()		 { return data + size; }
	T const	*end() const { return data + size; }
};

#endif