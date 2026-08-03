#include <UI/ButtonSettingUI.hpp>
#include <Input/SystemInputID.hpp>

ButtonSettingUI::ButtonSettingUI(const StaticText32& name, Point2_i position, OnPressFunc on_press)
	: ISettingUI{ name, position }, on_press{ on_press }
{
	delimiter = false;
}

bool ButtonSettingUI::ProcessInput(const AppEvent& event)
{
	// Если не выделен и не захвачен, обработка передается следующему элементу в списке
	// + в инкапсуляции состояния, и отсутствии необходимости извне опрашивать состояние кнопки
	if (!IsSelected())
		return false;

	if (IsSystemEnterEvent(event) && on_press)
		on_press();

	return false;
}
