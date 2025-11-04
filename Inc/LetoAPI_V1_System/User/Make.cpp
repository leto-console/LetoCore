#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

// ====================================================================================================

#include <Auth/AuthHandler.hpp>
#include <Bitmaps/Avatars.hpp>

NamedSpace* _GetUserSpace()
{
    NamedSpace* space{};
    AuthHandler::Instance().GetCurrentSpace(space);
    return space;
}

bool _GetAvatarByID(uint32_t ID, BitmapData* data)
{
    if (!data) return false;
    return GetAvatarByID(ID, *data);
}

// ====================================================================================================

const UserAPI_V1* Make_UserAPI()
{
    static const UserAPI_V1 api
    {
        &_GetUserSpace,
        &_GetAvatarByID
    };
    
    return &api;
}
