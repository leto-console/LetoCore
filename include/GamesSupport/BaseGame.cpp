#include "BaseGame.hpp"

ISceneManager* LoadedGame;

static void* Alloc(uint32_t size)
{
    if (!LoadedGame) return nullptr;
    return LoadedGame->GetCommonAllocator().Alloc(size);
}

static void Free(const void* ptr)
{
    if (!LoadedGame) return;
    LoadedGame->GetCommonAllocator().Free(ptr);
}

const LetoAllocator_V1* LoadedGameAllocator()
{
    static const LetoAllocator_V1 allocator
    {
        &Alloc,
        &Free
    };
    return &allocator;
}
