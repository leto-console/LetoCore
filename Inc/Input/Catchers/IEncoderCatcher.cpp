#include "IEncoderCatcher.hpp"

void IEncoderCatcher::Reset()
{
}

void IEncoderCatcher::Catch(uint8_t _encoder_id, uint16_t _mode)
{ 
    this->encoder_id = _encoder_id;
    this->mode = _mode;
}

void IEncoderCatcher::Loop()
{
    // TODO: handle mode
}

bool IEncoderCatcher::ProcessInput(const AppEvent &event)
{
    if (event.id != encoder_id)
        return false;
    
    if (EncoderEvent::IsTurnedLeft(event))
    {
        if (mode & ECM_ROTATE) Callback(true);
        return true;
    }
    else if (EncoderEvent::IsTurnedRight(event))
    {
        if (mode & ECM_ROTATE) Callback(false);
        return true;
    }
    
    return false;
}
