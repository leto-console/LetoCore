#include "IButtonCatcher.hpp"

#include <Input/ButtonEvent.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

void IButtonCatcher::Reset()
{
    pressed = holded = false;
    last_click_ms = 0;
}

void IButtonCatcher::Catch(uint8_t _button_id, uint16_t _mode)
{ 
    button_id.Push(_button_id);
    this->mode = _mode;
}

void IButtonCatcher::SetHoldTime(uint32_t _hold_ms, uint32_t _multiply_ms)
{
    this->hold_ms = _hold_ms;
    this->multiply_ms = _multiply_ms;
}

void IButtonCatcher::Loop()
{
    if (!pressed) return;

    uint32_t now_ms = leto_api_v1->Globals->GetCurrentMs();

    if (now_ms - last_click_ms > hold_ms)
    {
        if (!holded)
        {
            if (mode & BCM_HOLD) Callback();
            holded = true;
            multiply_timer.Start(multiply_ms);
        }
        
        if (multiply_timer.Expired())
        {
            if (mode & BCM_MULTI_HOLD) Callback();
            multiply_timer.Start();
        }
    }
}

bool IButtonCatcher::ProcessInput(const AppEvent &event)
{
    if (!button_id.Contains(event.id))
        return false;
    
    uint32_t now_ms = leto_api_v1->Globals->GetCurrentMs();

    if (ButtonEvent::IsPressed(event))
    {
        if (!pressed)
        {
            if (mode & BCM_SINGLE_PRESS) Callback();
            if (now_ms - last_click_ms < double_ms)
            {
                if (mode & BCM_DOUBLE_CLICK) Callback();
            }
            pressed = true;
            holded = false;
            last_click_ms = leto_api_v1->Globals->GetCurrentMs();
        }
        return true;
    }
    else if (ButtonEvent::IsReleased(event))
    {
        if (pressed)
        {
            if (mode & BCM_SINGLE_RELEASE) Callback();
            pressed = holded = false;
        }
        return true;
    }
    
    return false;
}
