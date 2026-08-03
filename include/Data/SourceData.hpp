/**
 * @file SourceData.hpp
 * @date Oct 13, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_SOURCE_DATA_HPP_
#define INC_DATA_SOURCE_DATA_HPP_

#include <Data/DataStates.hpp>
#include <cstring>
#include <cinttypes>


template <typename T, size_t Sources = 4>
class SourceData
{
protected:
	T values[Sources]{};
	DataStates states[Sources]{};
public:
	SourceData() 
	{
	}
	void SetBySource(T value, size_t src, DataStates state = DataStates::__OK)
	{
		values[src] = value;
		states[src] = state;
	}
	void InvalidateBySource(size_t src, DataStates state = DataStates::__TIMEOUT)
	{
		states[src] = state;
	}
	bool Get(T& value, DataStates* state = nullptr) const
	{
		for (size_t src = 0; src < Sources; src++)
			if (states[src] == DataStates::__OK) return GetBySource(value, src, state);
		return false;
	}
	bool GetBySource(T& value, size_t src, DataStates* state = nullptr) const
	{
		value = values[src];
		if (state) *state = states[src];
		return states[src] == DataStates::__OK;
	}
};

using BoolSourceData	= SourceData<bool>;
using U8SourceData		= SourceData<uint8_t>;
using U16SourceData		= SourceData<uint16_t>;
using U32SourceData		= SourceData<uint32_t>;
using FloatSourceData	= SourceData<float>;

#endif /* INC_DATA_SOURCE_DATA_HPP_ */
