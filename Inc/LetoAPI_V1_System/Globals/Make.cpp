#include "Make.hpp"

// ====================================================================================================

#include <System/CommonAllocator.hpp>
#include <System/DebugMode.hpp>
#include <Time/TimeUtils.hpp>
#include <System/DeviceID.hpp>

void* Alloc(uint32_t size)
{
    return CommonAllocator.Alloc(size);
}

void Free(const void* ptr)
{
    CommonAllocator.Free(ptr);
}

IAllocator* GetAllocator()
{
    return &CommonAllocator;
}

uint32_t GetCurrentMs()
{
    return TimeUtils::GetCurrentMs();
}

// ====================================================================================================

const GlobalsAPI_V1* Make_GlobalsAPI()
{
    static const GlobalsAPI_V1 api
    {
        &Alloc,
        &Free,
        &GetAllocator,
        &GetDebugMode,
        &GetCurrentMs,
        &GetDeviceID
    };
	
    return &api;
}
