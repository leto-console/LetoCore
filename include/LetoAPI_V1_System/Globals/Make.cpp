#include "Make.hpp"

// ====================================================================================================

#include <System/DebugMode.hpp>
#include <Time/TimeUtils.hpp>
#include <System/DeviceID.hpp>
#include <System/SystemLanguage.hpp>
#include <Utils/crc16.hpp>

#include <SceneManager/SystemSceneManager.hpp>
#include <AppLoader/AppLoader.hpp>

#include <utility>

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

static bool GetAppDir(char* buffer, uint32_t length)
{
    if (!buffer || length == 0) return false;
    if (!CurrentLoadedApp)
    {
        buffer[0] = '\0';
        return false;
    }

    const char* full_path = CurrentLoadedAppInfo.path;

    const char* last_slash = strrchr(full_path, '/');
    const char* last_backslash = strrchr(full_path, '\\');
    const char* last_sep = (last_slash > last_backslash) ? last_slash : last_backslash;

    uint32_t dir_len = 0;

    if (last_sep) 
    {
        // Включаем сам разделитель в путь (например, "folder/" или "C:\")
        dir_len = (uint32_t)(last_sep - full_path) + 1;
    }
    else 
    {
        buffer[0] = '\0';
        return false;
    }

    // Проверка на усечение
    if (dir_len >= length) 
    {
        memcpy(buffer, full_path, length - 1);
        buffer[length - 1] = '\0';
        return false;
    }

    memcpy(buffer, full_path, dir_len);
    buffer[dir_len] = '\0';
    return true;
}

// ====================================================================================================

const GlobalsAPI_V1* Make_GlobalsAPI()
{
    static const GlobalsAPI_V1 api
    {
        &GetAllocator,
        &GetDebugMode,
        &GetCurrentMs,
        &GetDeviceID,
        &calc_crc16,
        &GetAppDir,
        &GetSystemLanguage
    };
	
    return &api;
}
