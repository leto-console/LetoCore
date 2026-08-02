#include "Make.hpp"

#include <cstdio>
#include <cstdarg>

// ====================================================================================================

#include <Auth/AuthHandler.hpp>
#include <Bitmaps/Avatars.hpp>

const LetoBitmap_V1* _GetAvatarByID(uint32_t ID)
{
    return BitmapData::ToHandle(GetAvatarByID(ID));
}

//NamedSpace* _GetUserSpace()
//{
//    NamedSpace* space{};
//    AuthHandler::Instance().GetCurrentSpace(space);
//    return space;
//}

// ====================================================================================================

const UserAPI_V1* Make_UserAPI()
{
    static const UserAPI_V1 api
    {
        &_GetAvatarByID
        //&_GetUserSpace,
    };
    
    return &api;
}
