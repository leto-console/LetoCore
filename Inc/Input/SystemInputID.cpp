#include <Input/SystemInputID.hpp>

#include <Input/ButtonEvent.hpp>
#include <Input/EncoderEvent.hpp>
#include <Input/DiscreteSignalEventTypes.hpp>

bool IsSystemLeftEvent(const AppEvent& event, bool pressed)
{
	switch (event.source)
	{
	case AE_Button:
		return event.id == SYSTEM_BTN_LEFT && ButtonEvent::IsPressed(event, pressed);
	default:
		break;
	}
	return false;
}

bool IsSystemRightEvent(const AppEvent& event, bool pressed)
{
	switch (event.source)
	{
	case AE_Button:
		return event.id == SYSTEM_BTN_RIGHT && ButtonEvent::IsPressed(event, pressed);
	default:
		break;
	}
	return false;
}

bool IsSystemPrevEvent(const AppEvent& event, bool pressed)
{
	switch (event.source)
	{
	case AE_Button:
		return event.id == SYSTEM_BTN_UP && ButtonEvent::IsPressed(event, pressed);
	case AE_Encoder:
		return event.id == SYSTEM_ENC_MAIN && event.data == (uint8_t)EncoderEvent::TURN_LEFT;
	default:
		break;
	}
	return false;
}

bool IsSystemNextEvent(const AppEvent& event, bool pressed)
{
	switch (event.source)
	{
	case AE_Button:
		return event.id == SYSTEM_BTN_DOWN && ButtonEvent::IsPressed(event, pressed);
	case AE_Encoder:
		return event.id == SYSTEM_ENC_MAIN && event.data == (uint8_t)EncoderEvent::TURN_RIGHT;
	default:
		break;
	}
	return false;
}

bool IsSystemReturnEvent(const AppEvent& event, bool pressed)
{
	switch (event.source)
	{
	case AE_Button:
		return event.id == SYSTEM_BTN_LEFT && ButtonEvent::IsPressed(event, pressed);
	default:
		break;
	}
	return false;
}

bool IsSystemEnterEvent(const AppEvent& event, bool pressed)
{
	switch (event.source)
	{
	case AE_Button:
		return (event.id == SYSTEM_BTN_RIGHT || event.id == SYSTEM_BTN_ENTER) && ButtonEvent::IsPressed(event, pressed);
	default:
		break;
	}
	return false;
}

bool IsSystemMenuEvent(const AppEvent &event, bool pressed)
{
	switch (event.source)
	{
	case AE_Button:
		return event.id == SYSTEM_BTN_MENU && ButtonEvent::IsPressed(event, pressed);
	default:
		break;
	}
	return false;
}
