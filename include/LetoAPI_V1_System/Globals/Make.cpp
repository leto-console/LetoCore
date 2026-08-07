#include "Make.hpp"

// ====================================================================================================

#include <System/DebugMode.hpp>
#include <Time/TimeUtils.hpp>
#include <System/DeviceID.hpp>

#include <SceneManager/SystemSceneManager.hpp>

static void* Alloc(uint32_t size)
{
    return SystemSceneManager::Instance().GetCommonAllocator().Alloc(size);
}

static void Free(const void* ptr)
{
    SystemSceneManager::Instance().GetCommonAllocator().Free(ptr);
}

static const LetoAllocator_V1* GetAllocator()
{
    static const LetoAllocator_V1 allocator
    {
        &Alloc,
        &Free
    };

    return &allocator;
}

static uint32_t GetCurrentMs()
{
    return TimeUtils::GetCurrentMs();
}

// ====================================================================================================

const GlobalsAPI_V1* Make_GlobalsAPI()
{
    static const GlobalsAPI_V1 api
    {
        &GetAllocator,
        &GetDebugMode,
        &GetCurrentMs,
        &GetDeviceID
    };
	
    return &api;
}
