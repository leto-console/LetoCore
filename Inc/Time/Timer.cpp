#include "Time/Timer.hpp"

#include <LetoAPI_V1/LetoAPI_V1.hpp>

Timer::Timer(uint32_t period_ms)
	: period_ms{ period_ms }
{
}

void Timer::Start()
{
	if (leto_api_v1)
		start_ms = leto_api_v1->Globals->GetCurrentMs();
	else
		start_ms = 0;
	return_expired = false;
}

void Timer::Start(uint32_t period_ms)
{
	this->period_ms = period_ms;
	Start();
}

float Timer::GetProgress(bool bound) const
{
	if (period_ms == 0.0f)
		return 0.0f;

	float progress = (float) (leto_api_v1->Globals->GetCurrentMs() - start_ms) / period_ms;

	if (progress > 1.0f && bound)
		progress = 1.0f;

	return progress;
}

bool Timer::Expired(bool once) const
{
	if (return_expired && once)
		return false;
	bool expired = leto_api_v1->Globals->GetCurrentMs() - start_ms > period_ms;
	if (expired)
		return_expired = true;
	return expired;
}
