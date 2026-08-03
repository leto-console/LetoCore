#include "FatFsTask.hpp"
#include "FatFsMnt.hpp"
#include "FatFsTest.hpp"

#include "low_level/ff.h"

#include <VirtualConsole/VirtualConsole.hpp>

#ifdef USE_HAL_DRIVER
#include <SDCard/sdcard.h>
#endif


bool FatFsTask::RefreshHardware()
{
#ifndef USE_HAL_DRIVER
    return true;
#else
    return sdcard && sdcard->GetStatus() == ExtDeviceStatus::READY;
#endif
}

void FatFsTask::RefreshMount()
{
    if (fatfs_mounted) return;

    FRESULT res;
    // mount the default drive
    res = f_mount(&fatfs, "", 1);
    if(res != FR_OK) 
    {
        VC_Printf("f_mount() failed\r\n", RedColor);
        VC_Printf("res = %d\r\n", RedColor, res);
        return;
    }

    VC_Printf("f_mount() done!\r\n", GreenColor);
    fatfs_test();

    fatfs_mounted = true;
}

bool FatFsTask::Do()
{
    if (RefreshHardware())
    {
        RefreshMount();
    }
    else
    {
        if (fatfs_mounted)
            f_mount(NULL, "", 1);
        if (fatfs_mounted || fatfs_first_check)
            VC_Printf("hardware FF failed\r\n", RedColor);
        fatfs_mounted = false;
    }
    fatfs_first_check = false;
    return true;
}

FatFsTask::FatFsTask(const StaticText32 &name, uint32_t period_ms, uint8_t priority, SDCard_ExtDevice* sdcard)
    : PriorityTask{ name, period_ms, priority }, sdcard{ sdcard }
{
}
