#include "IButtonCatcher.hpp"

#include <Input/ButtonEvent.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

void IButtonCatcher::Reset()
{
    multiplied = pressed = holded = false;
    last_click_ms = 0;
    memset(button_pressed, 0, sizeof(button_pressed));
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
        if (!multiplied)
        {
            if (mode & BCM_HOLD_MULTIPLY)
            {
                bool multiply = true;
                for (uint8_t idx = 0; idx < button_id.size(); ++idx)
                {
                    if (!button_pressed[idx])
                    {
                        multiply = false;
                    }
                }
                if (multiply)
                {
                    Callback();
                    multiplied = true;
                }
            }
        }

        if (!holded)
        {
            if (mode & BCM_HOLD) Callback();
            holded = true;
            repeatition_timer.Start(multiply_ms);
        }
        
        if (repeatition_timer.Expired())
        {
            if (mode & BCM_HOLD_REPETITION) Callback();
            repeatition_timer.Start();
        }
    }
}

bool IButtonCatcher::ProcessInput(const AppEvent &event)
{
    uint8_t button_idx{};
    if (!GetIdxByID(button_idx, event.id))
        return false;
        
    uint32_t now_ms = leto_api_v1->Globals->GetCurrentMs();

    if (ButtonEvent::IsPressed(event))
    {
        button_pressed[button_idx] = true;
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
        button_pressed[button_idx] = false;
        multiplied = false;
        if (pressed)
        {
            if (mode & BCM_SINGLE_RELEASE) Callback();
            pressed = holded = false;
        }
        return true;
    }
    
    return false;
}

bool IButtonCatcher::GetIdxByID(uint8_t& out_idx, uint32_t id) const
{
    for (uint8_t idx = 0; idx < button_id.size(); ++idx)
    {
        if (button_id[idx] == id)
        {
            out_idx = idx;
            return true;
        }
    }
    return false;
}
