#pragma once

#if defined(__STM32__)

#include <LetoAPI_V1/LetoAPI_V1.h>
#include <FatFs/low_level/ff.h>
#include <FatFs/FatFsMnt.hpp>
#include <cstring>

namespace FileHandlerFatFs
{
    static FIL* FromHandle(LetoFile_V1* file) { return reinterpret_cast<FIL*>(file); }
    static LetoFile_V1* ToHandle( FIL* file) { return reinterpret_cast<LetoFile_V1*>(file); }

    extern FIL* CreateFromPool();
    extern void ReturnToPool(FIL* fil);
    extern void ClearPool();

    static BYTE GetModeText(LetoFileMode_V1 mode)
    {
        switch(mode)
        {
            case LFM_V1_WRITE:
                return FA_WRITE | FA_CREATE_ALWAYS;
            case LFM_V1_APPEND:
                return FA_WRITE | FA_OPEN_APPEND;
            case LFM_V1_READ:
            default:
                return FA_READ;
        };
    }

    static uint32_t Scan(const char* path, FileInfo_V1* array, uint32_t size)
    {
        if (!fatfs_mounted || !path || !array || size == 0) return 0;

        DIR dir;
        FILINFO fno;

        FRESULT res = f_opendir(&dir, path);
        if (res != FR_OK) return 0;

        uint32_t count = 0;

        while (count < size)
        {
            res = f_readdir(&dir, &fno);
            
            if (res != FR_OK || fno.fname[0] == 0) break;

            // Пропускаем текущую (.) и родительскую (..) директории
            if (fno.fname[0] == '.' && 
                (fno.fname[1] == 0 || (fno.fname[1] == '.' && fno.fname[2] == 0)))
            {
                continue;
            }

            FileInfo_V1& info = array[count];

            strncpy(info.path, path, sizeof(info.path) - 1);
            info.path[sizeof(info.path) - 1] = '\0';

            strncpy(info.filename, fno.fname, sizeof(info.filename) - 1);
            info.filename[sizeof(info.filename) - 1] = '\0';

            info.size = fno.fsize; // size=0 for AM_DIR
            
            ++count;
        }

        f_closedir(&dir);
        return count;
    }

    static LetoFile_V1* Open(const char* path, LetoFileMode_V1 mode)
    {
        if (!fatfs_mounted || !path) return nullptr;

        FIL* fil = CreateFromPool();
        if (!fil) return nullptr;

        FRESULT result = f_open(fil, path, GetModeText(mode));
        
        if (result != FR_OK)
        {
            ReturnToPool(fil);            
            return nullptr;
        }

        return ToHandle(fil);
    }

    static int8_t Close(LetoFile_V1* file)
    {
        if (!fatfs_mounted || !file) return LFV1_ERROR;

        FIL* fil = FromHandle(file);
        FRESULT res = f_close(fil);
        
        ReturnToPool(fil);
        
        return (res == FR_OK) ? 0 : LFV1_ERROR;
    }

    static int8_t Delete(const char* path)
    {
        if (!fatfs_mounted || !path) return LFV1_ERROR;

        FRESULT res = f_unlink(path);
        
        return (res == FR_OK) ? 0 : LFV1_ERROR;
    }

    static int32_t Read(LetoFile_V1* file, void* buffer, uint32_t size)
    {
        if (!fatfs_mounted || !file || !buffer || size == 0) return LFV1_ERROR;

        FIL* fil = FromHandle(file);
        UINT br = 0;
        FRESULT res = f_read(fil, buffer, size, &br);
        
        if (res != FR_OK) return LFV1_ERROR;
        return static_cast<int32_t>(br);
    }

    static int32_t Write(LetoFile_V1* file, const void* buffer, uint32_t size)
    {
        if (!fatfs_mounted || !file || !buffer || size == 0) return LFV1_ERROR;

        FIL* fil = FromHandle(file);
        UINT bw = 0;
        FRESULT res = f_write(fil, buffer, size, &bw);
        
        if (res != FR_OK) return LFV1_ERROR;
        return static_cast<int32_t>(bw);
    }

    static int8_t Flush(LetoFile_V1* file)
    {
        if (!fatfs_mounted || !file) return LFV1_ERROR;

        FIL* fil = FromHandle(file);
        FRESULT res = f_sync(fil);
        
        return (res == FR_OK) ? 0 : LFV1_ERROR;
    }

    static int8_t Seek(LetoFile_V1* file, int32_t pos, LetoFileSeek_V1 origin)
    {
        if (!fatfs_mounted || !file) return LFV1_ERROR;

        FIL* fil = FromHandle(file);
        
        FSIZE_t current_pos = f_tell(fil);
        FSIZE_t file_size = f_size(fil);
        FSIZE_t target_pos = 0;

        switch (origin) {
            case LFS_V1_SEEK_SET: // SEEK_SET
            {
                if (pos < 0) return LFV1_ERROR;
                target_pos = pos;
                break;
            }
            case LFS_V1_SEEK_CUR: // SEEK_CUR
            {
                if (pos > 0) 
                    target_pos = current_pos + pos;
                else 
                {
                    // Защита от ухода в минус от текущей позиции
                    if (static_cast<FSIZE_t>(-pos) > current_pos) 
                        return LFV1_ERROR;
                    target_pos = current_pos - static_cast<FSIZE_t>(-pos);
                } 
                break;
            }
            case LFS_V1_SEEK_END: // SEEK_END
            {
                if (pos > 0) 
                    target_pos = file_size + pos;
                else
                {
                    // Защита от ухода в минус от конца файла
                    if (static_cast<FSIZE_t>(-pos) > file_size) 
                        return LFV1_ERROR;
                    target_pos = file_size - static_cast<FSIZE_t>(-pos);
                }
                break;
            }
            default:
                return LFV1_ERROR;
        }

        FRESULT res = f_lseek(fil, target_pos);
        return (res == FR_OK) ? 0 : LFV1_ERROR;
    }
        
    static int32_t Tell(LetoFile_V1* file)
    {
        if (!fatfs_mounted || !file) return LFV1_ERROR;

        FIL* fil = FromHandle(file);
        return static_cast<int32_t>(f_tell(fil));
    }
}

#endif
