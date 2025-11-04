#include "FatFsTest.hpp"

#include "FatFsMnt.hpp"
#include "low_level/ff.h"
#include <cstring>

#include <VirtualConsole/VirtualConsole.hpp>

void fatfs_test()
{
    FRESULT res;
    VC_Printf("Ready!\r\n", GreenColor);

    DWORD freeClust;
    FATFS* fs_ptr = &fatfs;
    // Warning! This fills fs.n_fatent and fs.csize!
    res = f_getfree("", &freeClust, &fs_ptr);
    if(res != FR_OK) 
    {
    	VC_Printf("f_getfree() failed, res = %d\r\n", RedColor, res);
        return;
    }

    VC_Printf("f_getfree() done!\r\n", GreenColor);

    uint32_t totalBlocks = (fatfs.n_fatent - 2) * fatfs.csize;
    uint32_t freeBlocks = freeClust * fatfs.csize;

    VC_Printf("Total blocks: %lu (%lu Mb)\r\n", BlueColor,
                totalBlocks, totalBlocks / 2000);
    VC_Printf("Free blocks: %lu (%lu Mb)\r\n", BlueColor,
                freeBlocks, freeBlocks / 2000);

    DIR dir;
    res = f_opendir(&dir, "/");
    if(res != FR_OK) 
    {
    	VC_Printf("f_opendir() failed, res = %d\r\n", RedColor, res);
        return;
    }

    FILINFO fileInfo;
    uint32_t totalFiles = 0;
    uint32_t totalDirs = 0;
    VC_Printf("--------\r\nRoot directory:\r\n", BlueColor);
    for(;;) 
    {
        res = f_readdir(&dir, &fileInfo);
        if((res != FR_OK) || (fileInfo.fname[0] == '\0')) 
        {
            break;
        }

        if(fileInfo.fattrib & AM_DIR) 
        {
        	VC_Printf("  DIR  %s\r\n", BlueColor, fileInfo.fname);
            totalDirs++;
        } 
        else 
        {
        	VC_Printf("  FILE %s\r\n", BlueColor, fileInfo.fname);
            totalFiles++;
        }
    }

    VC_Printf("(total: %lu dirs, %lu files)\r\n--------\r\n", BlueColor,
                totalDirs, totalFiles);

    res = f_closedir(&dir);
    if(res != FR_OK) 
    {
    	VC_Printf("f_closedir() failed, res = %d\r\n", RedColor, res);
        return;
    }

    VC_Printf("Writing to log.txt...\r\n", BlueColor);

    char writeBuff[128];
    snprintf(writeBuff, sizeof(writeBuff),
        "Total blocks: %lu (%lu Mb); Free blocks: %lu (%lu Mb)\r\n",
        totalBlocks, totalBlocks / 2000,
        freeBlocks, freeBlocks / 2000);

    FIL logFile;
    res = f_open(&logFile, "log.txt", FA_OPEN_APPEND | FA_WRITE);
    if(res != FR_OK) 
    {
    	VC_Printf("f_open() failed, res = %d\r\n", RedColor, res);
        return;
    }

    unsigned int bytesToWrite = strlen(writeBuff);
    unsigned int bytesWritten;
    res = f_write(&logFile, writeBuff, bytesToWrite, &bytesWritten);
    if(res != FR_OK) 
    {
    	VC_Printf("f_write() failed, res = %d\r\n", RedColor, res);
        return;
    }

    if(bytesWritten < bytesToWrite) 
    {
    	VC_Printf("WARNING! Disk is full.\r\n", RedColor);
    }

    res = f_close(&logFile);
    if(res != FR_OK) 
    {
    	VC_Printf("f_close() failed, res = %d\r\n", RedColor, res);
        return;
    }

    VC_Printf("Reading file...\r\n", BlueColor);
    FIL msgFile;
    res = f_open(&msgFile, "log.txt", FA_READ);
    if(res != FR_OK) 
    {
    	VC_Printf("f_open() failed, res = %d\r\n", RedColor, res);
        return;
    }

    char readBuff[128];
    unsigned int bytesRead;
    res = f_read(&msgFile, readBuff, sizeof(readBuff)-1, &bytesRead);
    if(res != FR_OK) 
    {
    	VC_Printf("f_read() failed, res = %d\r\n", RedColor, res);
        return;
    }

    readBuff[bytesRead] = '\0';
    VC_Printf("```\r\n%s\r\n```\r\n", BlueColor, readBuff);

    res = f_close(&msgFile);
    if(res != FR_OK) 
    {
    	VC_Printf("f_close() failed, res = %d\r\n", RedColor, res);
        return;
    }

    VC_Printf("Done!\r\n", BlueColor);
}
