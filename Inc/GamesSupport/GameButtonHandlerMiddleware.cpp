#include <GamesSupport/GameButtonHandlerMiddleware.hpp>

#include <Input/ButtonEvent.hpp>

void GameButtonHandlerMiddleware::HandleEvent(const AppEvent& event, IsEventFunc is_event, ButtonHoldHandler & handler)
{
	if (is_event(event, true))
		handler.Press();
	else if (is_event(event, false))
		handler.Release();
}

void GameButtonHandlerMiddleware::HandleEvent(const AppEvent& event, IsEventFunc is_event, ButtonMultiPressHandler & handler, int button)
{
	if (is_event(event, true))
		handler.Press(button);
	else if (is_event(event, false))
		handler.Release(button);
}
