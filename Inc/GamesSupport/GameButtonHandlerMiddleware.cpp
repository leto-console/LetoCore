#include <GamesSupport/GameButtonHandlerMiddleware.hpp>

#include <Input/ButtonEvent.hpp>

void GameButtonHandlerMiddleware::HandleEvent(const AppEvent& event, const AppEvent& source, ButtonHoldHandler & handler)
{
	if (event.source == source.source &&
		event.id == source.id)
	{
		if (ButtonEvent::IsPressed(event))
		{
			handler.Press();
		}
		else if (ButtonEvent::IsReleased(event))
		{
			handler.Release();
		}
	}
}

void GameButtonHandlerMiddleware::HandleEvent(const AppEvent& event, const AppEvent& source, ButtonMultiPressHandler & handler, int button)
{
	if (event.source == source.source &&
		event.id == source.id)
	{
		if (ButtonEvent::IsPressed(event))
		{
			handler.Press(button);
		}
		else if (ButtonEvent::IsReleased(event))
		{
			handler.Release(button);
		}
	}
}
