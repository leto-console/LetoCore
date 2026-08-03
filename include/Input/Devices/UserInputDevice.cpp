#include <Input/Devices/UserInputDevice.hpp>

#ifndef USE_HAL_DRIVER
std::list<uint64_t> UserInputDevice::keys;

bool UserInputDevice::KeyExists(uint64_t key)
{
	for (uint64_t added_keys : UserInputDevice::keys)
		if (added_keys == key)
			return true;

	return false;
}

#endif

RingFIFO_Static<AppEvent, 64> UserInputDevice::events;
