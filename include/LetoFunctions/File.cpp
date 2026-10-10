#include "File.hpp"

namespace leto::file
{

    FileGuard::FileGuard(const char* path, LetoFileMode_V1 mode)
    {
        file = leto_api_v1->File->Open(path, mode);
    }

    FileGuard::~FileGuard()
    {
        if (file) leto_api_v1->File->Close(file);
    }

    int32_t FileGuard::Write(const void* buffer, uint32_t size)
    {
        if (!file) return LFV1_ERROR;
        return leto_api_v1->File->Write(file, buffer, size);
    }

    int32_t FileGuard::Read(void* buffer, uint32_t size) const
    {
        if (!file) return LFV1_ERROR;
        return leto_api_v1->File->Read(file, buffer, size);
    }

    int8_t FileGuard::Flush()
    {
        if (!file) return LFV1_ERROR;
        return leto_api_v1->File->Flush(file);
    }

    int8_t FileGuard::Seek(int32_t pos, LetoFileSeek_V1 origin)
    {
        if (!file) return LFV1_ERROR;
        return leto_api_v1->File->Seek(file, pos, origin);
    }
        
    int32_t FileGuard::Tell() const
    {
        if (!file) return LFV1_ERROR;
        return leto_api_v1->File->Tell(file);
    }

}
