#pragma once

#include "LibrariesExport.h"
#include <LetoAPI_V1/LetoAPI_V1.h>

namespace leto::file
{
    /**
     * @brief RAII wrapper for automatic file lifecycle management.
     * 
     * Opens the file upon construction and guarantees its safe closure 
     * when leaving the scope. Non-copyable and non-movable.
     */
    class LETO_CORE_EXPORT FileGuard
    {
    protected:
        LetoFile_V1* file = nullptr;
    
    public:
        FileGuard(const char* path, LetoFileMode_V1 mode);
        ~FileGuard();

        FileGuard(const FileGuard& other) = delete;
        FileGuard& operator=(const FileGuard& other) = delete;

        FileGuard(FileGuard&& other) = delete;
        FileGuard& operator=(FileGuard&& other) = delete;

        bool IsOpen() const { return file != nullptr; }

        LetoFile_V1*        GetFile()       { return file; }
        LetoFile_V1 const*  GetFile() const { return file; }

        int32_t Write(const void* buffer, uint32_t size);
        int32_t Read(void* buffer, uint32_t size) const;
        int8_t Flush();
        int8_t Seek(int32_t pos, LetoFileSeek_V1 origin);
        int32_t Tell() const;
    };

    /**
     * @brief Scan the directory and get info
     *
     * @param[in] path Directory path
     * @param[out] array Array of FileInfo_V1 structures
     * @param[in] size Size of the array (max number of entries to fill)
     *
     * @return Count of found items (can be greater than 'size' if directory contains more files)
     */
    inline uint32_t Scan(const char* path, FileInfo_V1* array, uint32_t size)
    {
        return leto_api_v1->File->Scan(path, array, size);
    }

    /**
     * @brief Open the file
     *
     * @param[in] path File path
     * @param[in] mode Open mode
     *
     * @return File handle ('NULL' if error)
     */
    inline LetoFile_V1* Open(const char* path, LetoFileMode_V1 mode)
    {
        return leto_api_v1->File->Open(path, mode);
    }
    
    /**
     * @brief Close the file
     *
     * @param[in] file File handle
     *
     * @return Result (`-1` if error, `0` if successful)
     */
    inline int8_t Close(LetoFile_V1* file)
    {
        return leto_api_v1->File->Close(file);
    }

    /**
     * @brief Delete the file
     *
     * @param[in] path File or empty directory path
     *
     * @return Result (`-1` if error, `0` if successful)
     */
    inline int8_t Delete(const char* path)
    {
        return leto_api_v1->File->Delete(path);
    }

    /**
     * @brief Read data from the file
     *
     * @param[in] file File handle
     * @param[out] buffer Data buffer
     * @param[in] size Size of data buffer
     *
     * @return Number of bytes read (`-1` if error)
     */
    inline int32_t Read(LetoFile_V1* file, void* buffer, uint32_t size)
    {
        return leto_api_v1->File->Read(file, buffer, size);
    }
    
    /**
     * @brief Write data to the file
     *
     * @param[in] file File handle
     * @param[in] buffer Data buffer
     * @param[in] size Size of data buffer
     *
     * @return Number of bytes written (`-1` if error)
     */
    inline int32_t Write(LetoFile_V1* file, const void* buffer, uint32_t size)
    {
        return leto_api_v1->File->Write(file, buffer, size);
    }

    /**
     * @brief Flush write buffers to physical media
     *
     * @param[in] file File handle
     *
     * @return Result (`-1` if error, `0` if successful)
     */
    inline int8_t Flush(LetoFile_V1* file)
    {
        return leto_api_v1->File->Flush(file);
    }

    /**
     * @brief Seek read/write offset 
     *
     * @param[in] file File handle
     * @param[in] pos Offset value
     * @param[in] origin Origin position (e.g., start, current, end)
     *
     * @return Result (`-1` if error, `0` if successful)
     */
    inline int8_t Seek(LetoFile_V1* file, int32_t pos, LetoFileSeek_V1 origin)
    {
        return leto_api_v1->File->Seek(file, pos, origin);
    }
        
    /**
     * @brief Get current read/write offset 
     *
     * @param[in] file File handle
     *
     * @return Current read/write offset (`-1` if error)
     */
    inline int32_t Tell(LetoFile_V1* file)
    {
        return leto_api_v1->File->Tell(file);
    }

}