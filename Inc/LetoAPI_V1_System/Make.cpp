#include "Make.hpp"

#include "Text/Make.hpp"
#include "Support/Make.hpp"
#include "Globals/Make.hpp"
#include "Web/Make.hpp"
#include "User/Make.hpp"
#include "Font/Make.hpp"
#include "Math/Make.hpp"
#include "Lobby/Make.hpp"

const LetoAPI_V1* Make_LetoAPI_V1()
{
    static const LetoAPI_V1::VersionAPI version
    {
        1,
        LETO_API_V1_MINOR
    };

    static const LetoAPI_V1 api
    {
        &version,
        Make_SupportAPI(),
        Make_TextAPI(),
        Make_GlobalsAPI(),
        Make_WebAPI(),
        Make_UserAPI(),
        Make_FontAPI(),
        Make_MathAPI(),
        Make_LobbyAPI()
    };

    leto_api_v1 = &api;
    
    return &api;
}
