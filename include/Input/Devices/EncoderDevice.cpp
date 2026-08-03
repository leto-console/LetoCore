#include <Input/Devices/EncoderDevice.hpp>

#include <Input/EncoderEvent.hpp>

EncoderDevice::EncoderDevice()
	: UserInputDevice{ AE_Encoder }
{
}

#ifdef USE_HAL_DRIVER
void EncoderDevice::Init(uint8_t id, GPIO_TypeDef* gpio_clk_port, uint16_t gpio_clk_pin, GPIO_TypeDef* gpio_dt_port, uint16_t gpio_dt_pin)
{
	this->gpio_clk_port = gpio_clk_port;
	this->gpio_clk_pin = gpio_clk_pin;
	this->gpio_dt_port = gpio_dt_port;
	this->gpio_dt_pin = gpio_dt_pin;

	UserInputDevice::Init(id);
}
#else
void EncoderDevice::Init(uint8_t id, uint64_t key_left, uint64_t key_right)
{
	this->keys.push_back(key_left);
	this->keys.push_back(key_right);

	this->key_left = key_left;
	this->key_right = key_right;

	UserInputDevice::Init(id);
}
#endif

void EncoderDevice::Tick()
{
	if (!initialized)
		return;

#ifdef USE_HAL_DRIVER
	stateCLK = !HAL_GPIO_ReadPin(gpio_clk_port, gpio_clk_pin);
	stateDT = !HAL_GPIO_ReadPin(gpio_dt_port, gpio_dt_pin);

	if (stateCLK && stateCLK != lastStateCLK) {
		if ((stateDT != stateCLK) != reverse)
			OnLeftTurn();
		else
			OnRightTurn();
	}

	lastStateCLK = stateCLK;

#elif defined(__WIN__)

	if (WinAPIEventsMap.find(key_left) != WinAPIEventsMap.end() &&
		!WinAPIEventsMap[key_left].empty())
	{
		WinAPIKeyEvent& event = WinAPIEventsMap[key_left].front();
		if (event.event == WinAPIKeyEvent::PRESSED) OnLeftTurn();
		WinAPIEventsMap[key_left].pop_front();
	}

	if (WinAPIEventsMap.find(key_right) != WinAPIEventsMap.end() &&
		!WinAPIEventsMap[key_right].empty())
	{
		WinAPIKeyEvent& event = WinAPIEventsMap[key_right].front();
		if (event.event == WinAPIKeyEvent::PRESSED) OnRightTurn();
		WinAPIEventsMap[key_right].pop_front();
	}

#endif
}

void EncoderDevice::SetReverse(bool reverse)
{
	this->reverse = reverse;
}

void EncoderDevice::OnLeftTurn(uint8_t count)
{
	PushEvent((uint8_t) EncoderEvent::TURN_LEFT);
}

void EncoderDevice::OnRightTurn(uint8_t count)
{
	PushEvent((uint8_t) EncoderEvent::TURN_RIGHT);
}
