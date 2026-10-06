#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

#if defined(__WIN__)
#include "FileHandlerWin.hpp"
using namespace FileHandlerWin;
#elif defined(__STM32__)
#include "FileHandlerFatFs.hpp"
using namespace FileHandlerFatFs;
#endif

// ====================================================================================================

const FileAPI_V1* Make_FileAPI()
{
    static const FileAPI_V1 api
    {
        &Scan,
        &Open,
        &Close,
        &Delete,
        &Read,
        &Write,
        &Flush,
        &Seek,
        &Tell
    };
    
    return &api;
}
