#include <UI/Animation/Animation.hpp>

Animation::Animation(int duration_ms)
	: duration_ms{ duration_ms }
{
}

bool Animation::IsPlaying() const
{
	return state == State::PLAY;
}

void Animation::Play()
{
	if (state != State::PAUSE)
		start_ms = leto_api_v1->Globals->GetCurrentMs();
	state = State::PLAY;
}
