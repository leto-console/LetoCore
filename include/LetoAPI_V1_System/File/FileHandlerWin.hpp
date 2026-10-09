#pragma once

#ifdef __WIN__

#include <LetoAPI_V1/LetoAPI_V1.h>

#include <Windows.h>
#include <cstdio>
#include <cstring>

namespace FileHandlerWin
{
    static FILE* FromHandle(LetoFile_V1* file) { return reinterpret_cast<FILE*>(file); }
    static LetoFile_V1* ToHandle( FILE* file) { return reinterpret_cast<LetoFile_V1*>(file); }

    static const char* GetModeText(LetoFileMode_V1 mode)
    {
        switch(mode)
        {
            case LFM_V1_WRITE:
                return "wb";
            case LFM_V1_APPEND:
                return "ab";
            case LFM_V1_READ:
            default:
                return "rb";
        };
    }

    static uint8_t GetSeekOrigin(LetoFileSeek_V1 origin)
    {
        switch(origin)
        {
            case LFS_V1_SEEK_SET: return SEEK_SET;  // С начала файла
            case LFS_V1_SEEK_CUR: return SEEK_CUR;  // От текущей позиции
            case LFS_V1_SEEK_END: return SEEK_END;  // С конца файла
            default: break;
        }
        return SEEK_SET;
    }

    static uint32_t Scan(const char* path, FileInfo_V1* array, uint32_t size)
    {
        if (!array || !size || !path)
            return 0;

        WIN32_FIND_DATAA file_data;
        uint32_t count = 0;

        char search_mask[260]; 
        snprintf(search_mask, sizeof(search_mask), "%s\\*", path);

        HANDLE hFind = FindFirstFileA(search_mask, &file_data);
        
        if (hFind == INVALID_HANDLE_VALUE) 
            return 0;

        do 
        {
            // Пропускаем текущую (.) и родительскую (..) директории
            if (strcmp(file_data.cFileName, ".") == 0 || strcmp(file_data.cFileName, "..") == 0)
                continue;

            if (count < size) 
            {
                snprintf(array[count].path, sizeof(array[count].path), "%s", path);
                snprintf(array[count].filename, sizeof(array[count].filename), "%s", file_data.cFileName);

                // Проверяем, что это файл, а не папка
                if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
                    array[count].size = ((uint64_t)file_data.nFileSizeHigh << 32) | file_data.nFileSizeLow;
                else
                    array[count].size = 0;
            }

            count++;
        } 
        while (FindNextFileA(hFind, &file_data));

        FindClose(hFind);
        return count;
    }

    static LetoFile_V1* Open(const char* path, LetoFileMode_V1 mode)
    {
        if (!path)
            return nullptr;
        FILE* f = fopen(path, GetModeText(mode));
        return ToHandle(f);
    }

    static int8_t Close(LetoFile_V1* file)
    {
        if (!file)
            return LFV1_ERROR;
        int result = fclose(FromHandle(file));
        return (result == 0) ? 0 : LFV1_ERROR;
    }

    static int8_t Delete(const char* path)
    {
        if (!path)
            return LFV1_ERROR;

        return DeleteFileA(path) ? 0 : LFV1_ERROR;
    }

    static int32_t Read(LetoFile_V1* file, void* buffer, uint32_t size)
    {
        if (!file || !buffer || size == 0)
            return LFV1_ERROR;
        size_t read = fread(buffer, 1, size, FromHandle(file));
        return static_cast<int32_t>(read);
    }

    static int32_t Write(LetoFile_V1* file, const void* buffer, uint32_t size)
    {
        if (!file || !buffer || size == 0)
            return LFV1_ERROR;
        size_t written = fwrite(buffer, 1, size, FromHandle(file));
        return static_cast<int32_t>(written);
    }

    static int8_t Flush(LetoFile_V1* file)
    {
        if (!file)
            return LFV1_ERROR;
        int result = fflush(FromHandle(file));
        return (result == 0) ? 0 : LFV1_ERROR;
    }

    static int8_t Seek(LetoFile_V1* file, int32_t pos, LetoFileSeek_V1 origin)
    {
        if (!file)
            return LFV1_ERROR;
        int result = fseek(FromHandle(file), pos, GetSeekOrigin(origin));
        return (result == 0) ? 0 : LFV1_ERROR;
    }
        
    static int32_t Tell(LetoFile_V1* file)
    {
        if (!file)
            return LFV1_ERROR;
        long pos = ftell(FromHandle(file));
        return static_cast<int32_t>(pos);
    }
}

#endif
