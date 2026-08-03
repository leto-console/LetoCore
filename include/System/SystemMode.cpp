#include <System/SystemMode.hpp>

static SystemMode _mode = SystemMode::NONE;
static SystemMode _last = SystemMode::NONE;

void SetSystemMode(SystemMode mode)
{
	// Пользователю разрешается только сменить режим на авторизацию
	if (_mode == SystemMode::USER &&
		mode != SystemMode::AUTH)
		return;
	_mode = mode;
}

SystemMode GetSystemMode()
{
	return _mode;
}

bool SystemModeChanged()
{
	bool chanded = _last != _mode;
	_last = _mode;
	return chanded;
}
