/**
 * @file SettingUI.hpp
 * @date Dec 17, 2025
 * @author Rakhimov T.
 */

#ifndef INC_UI_SETTING_UI_HPP_
#define INC_UI_SETTING_UI_HPP_

#include "LibrariesExport.h"

#include <Drawable/IDrawable.hpp>
#include <SceneManager/ISelectable.hpp>
#include <SceneManager/ICapturable.hpp>

#include <Data/StaticText.hpp>

#include <Time/Timer.hpp>

#include <UI/ButtonHoldHandler.hpp>
#include <Graphics/IFont.hpp>

class LIBRARIES_EXPORT ISettingUI : public IDrawable, public ISelectable, public ICapturable
{
protected:
	const StaticText32 N_A = "N/A";

	StaticText32 name;
	short name_value_offset = -1;
	short name_delim_offset = -1;

	bool delimiter = true;

	const IFont* setting_font{};

	// Текущее значение настройки
	virtual StaticText32 CurrentValueRepr() = 0;

	// Отрисовка названия настройки
	virtual void DrawName(IScreen& screen);

	// Отрисовка значения настройки
	virtual void DrawValue(IScreen& screen);

public:
	ISettingUI(const StaticText32& name, Point2_i position = {});
	virtual ~ISettingUI() = default;

	void SetFont(const IFont* setting_font);

	// Обновить текущее значение настройки (запрос из eeprom)
	virtual void UpdateCurrentValue() = 0;

	void Draw(IScreen& screen, Point2_i offset = {}) override;

	bool ProcessInput(const AppEvent& event) override;
};

 // Интерфейс графического элемента настроек 
class LIBRARIES_EXPORT IEditableSettingUI : public virtual ISettingUI
{
protected:
	// TODO: add blink
	//Timer capture_blink_timer;
	//bool blink_state;

	// Отображение настройки в UI определяется не Setting'ом, а отображающим классом
	// Соответственно маппинг производится тоже тут

	ButtonHoldHandler return_hold_handler;
	ButtonHoldHandler apply_hold_handler;

	// Текущее изменяемое значение настройки (в режиме захвата ввода)
	virtual StaticText32 EditingValueRepr() = 0;

	// Установить текущую измененную настройку
	virtual void SetEditingValue() = 0;

	// Обработка действий изменения
	virtual bool ProcessEditingEvent(const AppEvent& event) = 0;

	// Отрисовка значения настройки (в том числе в состоянии изменения
	virtual void DrawValue(IScreen& screen) override;

	// Применить редактирование
	virtual void ApplyEditing();

	// Отменить редактирование
	virtual void CancelEditing();

public:
	IEditableSettingUI(const StaticText32& name, Point2_i position = {});
	virtual ~IEditableSettingUI() = default;

	bool ProcessInput(const AppEvent& event) override;

	void Loop() override;
};

#endif
