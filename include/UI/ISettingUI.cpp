#include <UI/ISettingUI.hpp>

#include <Input/ButtonEvent.hpp>
#include <Input/SystemInputID.hpp>

#include <LetoAPI_V1/LetoAPI_V1.h> 
#include <Data/StaticText.hpp>
#include <DrawFunctions/DrawText.hpp>

// ====================================================================================================

void ISettingUI::DrawName(IScreen& screen)
{
	using namespace DrawFunctions;
	static StaticText<8> delim_symbol = ": ";

	// Инверсия цвета настройки при выделении, но не при редактировании
	bool reverse_name = IsSelected() && !IsCaptured();
	DrawText(screen, {position.x, position.y}, name, WhiteColor, BlackColor, reverse_name, setting_font);
	if (delimiter)
		DrawText(screen, {position.x + name_delim_offset, position.y}, delim_symbol, WhiteColor, BlackColor, false, setting_font);
}

void ISettingUI::DrawValue(IScreen& screen)
{
	using namespace DrawFunctions;
	StaticText32 repr = CurrentValueRepr();
	if (name_value_offset + repr.TextLength() * 8 > screen.Width())
	{
		SetHeight(16);
		DrawText(screen, {position.x, position.y + 8}, CurrentValueRepr(), WhiteColor, BlackColor, false, setting_font);
	}
	else
		DrawText(screen, {position.x + name_value_offset, position.y}, CurrentValueRepr(), WhiteColor, BlackColor, false, setting_font);
}

ISettingUI::ISettingUI(const StaticText32& name, Point2_i position)
	: name{ name }
{
	SetPosition(position);
	SetHeight(8);
}

void ISettingUI::SetFont(const IFont* setting_font)
{
	this->setting_font = setting_font;
}

void ISettingUI::Draw(IScreen& screen, Point2_i offset)
{
	using namespace DrawFunctions;

	if (name_value_offset < 0 || name_delim_offset < 0)
	{
		name_delim_offset = TextWidth(name);
		name_value_offset = TextWidth(name + ": ");
	}

	UpdateCurrentValue();
	DrawName(screen);
	DrawValue(screen);
}

bool ISettingUI::ProcessInput(const AppEvent& event)
{
	return false;
}

// ====================================================================================================

void IEditableSettingUI::DrawValue(IScreen& screen)
{
	using namespace DrawFunctions;

	if (IsCaptured())
	{
		// TODO: добавить blink, основанный на таймере
		// Инверсия заливки при редактировании
		// Прописать сюда заливку сначала белым вокруг, потом черным - поля, которое редактируется

		DrawText(screen, {position.x + name_value_offset, position.y}, EditingValueRepr(), WhiteColor, BlackColor, true, setting_font);
		return;
	}

	ISettingUI::DrawValue(screen);
}

void IEditableSettingUI::ApplyEditing()
{
	SetEditingValue();
	uint32_t cur = leto_api_v1->Globals->GetCurrentMs();
	while (leto_api_v1->Globals->GetCurrentMs() - cur < 30);
	UpdateCurrentValue();
	Capture(false);
}

void IEditableSettingUI::CancelEditing()
{
	UpdateCurrentValue();
	Capture(false);
}

IEditableSettingUI::IEditableSettingUI(const StaticText32& name, Point2_i position)
	: ISettingUI{ name, position }
{
	//capture_blink_timer.Start(700);
}

bool IEditableSettingUI::ProcessInput(const AppEvent& event)
 {
	// Если не выделен и не захвачен, обработка передается следующему элементу в списке
	// + в инкапсуляции состояния, и отсутствии необходимости извне опрашивать состояние кнопки
	if (!IsSelected())
		return false;

	if (!IsCaptured())
	{
		if (IsSystemEnterEvent(event, true))
		{
			Capture();
			return true;
		}
		return false;
	}

	bool process_editing_event = ProcessEditingEvent(event);

	// Если настройка захватывает нажатие кнопки Влево, 
	// для выхода используется удерживание
	if (IsSystemLeftEvent(event, true))
	{
		if (process_editing_event)
			return_hold_handler.Press();
		else
			CancelEditing();
		return true;
	}
	else if (IsSystemLeftEvent(event, false))
	{
		return_hold_handler.Release();
		return true;
	}

	if (IsSystemEnterEvent(event, true))
	{
		if (process_editing_event)
			apply_hold_handler.Press();
		else
			ApplyEditing();
		return true;
	}
	else if (IsSystemEnterEvent(event, false))
	{
		apply_hold_handler.Release();
		return true;
	}

	return process_editing_event;
}

void IEditableSettingUI::Loop()
{
	if (IsCaptured())
	{
		if (return_hold_handler.Holded(1000))
		{
			// Удержать кнопку влево - отмена настройки
			CancelEditing();
		}
		if (apply_hold_handler.Holded(1000))
		{
			// Удержать кнопку вправо - применить настройку
			ApplyEditing();
		}
	}
	else
	{
		return_hold_handler.Release();
		apply_hold_handler.Release();
	}
}
