/**
 * @file SingleData.hpp
 * @date Oct 19, 2025
 * @author Rakhimov T.
 */

#ifndef INC_DATA_SINGLE_DATA_HPP_
#define INC_DATA_SINGLE_DATA_HPP_

#include <Data/DataStates.hpp>
#include <cstring>
#include <cinttypes>


template <typename T, size_t Sources = 4>
class SingleData
{
protected:
	T _value{};
	DataStates _state{};
public:
	SingleData()
	{
	}
	void Set(T value, DataStates state = DataStates::__OK)
	{
		_value = value;
		_state = state;
	}
	void Invalidate(DataStates state = DataStates::__TIMEOUT)
	{
		_state = state;
	}
	bool Get(T& value, DataStates* state = nullptr) const
	{
		value = _value;
		if (state) *state = _state;
		return _state == DataStates::__OK;
	}
};

using BoolSingleData	= SingleData<bool>;
using U8SingleData		= SingleData<uint8_t>;
using U16SingleData		= SingleData<uint16_t>;
using U32SingleData		= SingleData<uint32_t>;
using FloatSingleData	= SingleData<float>;

#endif /* INC_DATA_SINGLE_DATA_HPP_ */
