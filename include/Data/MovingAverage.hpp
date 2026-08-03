/**
 * @file MovingAverage.hpp
 * @date Mar 12, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_MOVING_AVERAGE_HPP_
#define INC_DATA_MOVING_AVERAGE_HPP_

#include <Data/RingFIFO_Static.hpp>

// Скользящее среднее
template <typename T, int WindowWidth>
class MovingAverage
{
protected:
	RingFIFO_Static<T, WindowWidth> ring_fifo;

public:
	void Add(const T& value)
	{
		ring_fifo.Push(value);
	}

	float GetFloat() const
	{
		return 1.0f * ring_fifo.Sum() / ring_fifo.Count();
	}

	T Get() const
	{
		return (T) GetFloat();
	}

};

#endif