#include "FileHandlerFatFs.hpp"

#ifdef __STM32__

using namespace FileHandlerFatFs;

struct PoolItem
{
    FIL fil;
    bool used{};
};

static PoolItem pool[5] = {};

FIL *FileHandlerFatFs::CreateFromPool()
{
    for (PoolItem& item : pool)
    {
        if (!item.used)
        {
            item.used = true;
            return &item.fil;
        }
    }
    return nullptr;
}

void FileHandlerFatFs::ReturnToPool(FIL *fil)
{
    for (PoolItem& item : pool)
    {
        if (&item.fil == fil)
        {
            item.used = false;
            break;
        }
    }
}

void FileHandlerFatFs::ClearPool()
{
    for (PoolItem& item : pool)
    {
        item.used = false;
    }
}


#endif

