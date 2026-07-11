#include "IndigoScreen.hpp"

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>
#endif

#include <System/SystemDisplay.hpp>

void CrashIndigoScreen(const StaticText32& message, const StaticText32& data)
{
#ifdef USE_HAL_DRIVER
    __disable_irq();
#endif
    if (SystemDisplay && !DisplayStackGuard->Check()) 
    {
        char txt[20];
        IndigoScreenDrawer indigo;
        indigo.SetMessage(message);
        indigo.SetData(data);

        while (!SystemDisplay->Render(&indigo));
        SystemDisplay->Loop();
    }

    #ifdef USE_HAL_DRIVER
        while (1) { }
    #else
        throw message.ConstChar();
    #endif
}
