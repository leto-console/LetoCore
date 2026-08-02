/*
 * ButtonSettingUI.hpp
 *
 *  Created on: Jan 26, 2026
 *      Author: Timur
 */

#ifndef INC_UI_BUTTON_SETTING_UI_HPP_
#define INC_UI_BUTTON_SETTING_UI_HPP_

#include "LibrariesExport.h"

#include <UI/ISettingUI.hpp>
#include <Data/StaticText.hpp>

class LETO_CORE_EXPORT ButtonSettingUI : public ISettingUI
{
public:
	using OnPressFunc = void(*)();

protected:
	OnPressFunc on_press{ nullptr };

	StaticText32 CurrentValueRepr() { return ""; };

public:
	ButtonSettingUI(const StaticText32& name, Point2_i position, OnPressFunc on_press);

	void UpdateCurrentValue() {};

	bool ProcessInput(const AppEvent& event) override;
};

#include <Input/SystemInputID.hpp>

#include <type_traits>

template <typename Instance>
class ButtonCallInstanceSettingUI : public ISettingUI
{
public:
	using OnPressFunc = void(Instance::*)();

protected:
	Instance* instance{ nullptr };
	OnPressFunc on_press{ nullptr };

	StaticText32 CurrentValueRepr() { return ""; };

public:
	ButtonCallInstanceSettingUI(const StaticText32& name, Point2_i position, Instance* instance, OnPressFunc on_press)
		: ISettingUI{ name, position }, instance{ instance }, on_press{on_press}
	{
		delimiter = false;
	}

	ButtonCallInstanceSettingUI(const StaticText32& name, Point2_i position)
		: ISettingUI{ name, position }
	{
		delimiter = false;
	}

	void InitScene(Instance* instance, OnPressFunc on_press)
	{
		this->instance = instance;
		this->on_press = on_press;
	}

	void UpdateCurrentValue() {};

	bool ProcessInput(const AppEvent& event) override
	{
		// Если не выделен и не захвачен, обработка передается следующему элементу в списке
		// + в инкапсуляции состояния, и отсутствии необходимости извне опрашивать состояние кнопки
		if (!IsSelected())
			return false;

		if (IsSystemEnterEvent(event) && instance && on_press)
			(instance->*on_press)();

		return false;
	}
};

#include <tuple>
#include <Utils/static_tuple_apply.hpp>

template <typename... Arg>
class ButtonArgSettingUI : public ISettingUI
{
protected:
	using OnPressArgFunc = void(*)(Arg...);

	OnPressArgFunc on_press{ nullptr };
	static constexpr int size = sizeof...(Arg);
	std::tuple<Arg...> args;

	StaticText32 CurrentValueRepr() { return ""; };

public:
	ButtonArgSettingUI(const StaticText32& name, Point2_i position, OnPressArgFunc on_press, Arg... args)
		: ISettingUI{ name, position }, on_press{ on_press }, args{ args... }
	{
		delimiter = false;
	}

	void UpdateCurrentValue() {};

	bool ProcessInput(const AppEvent& event) override
	{
		// Если не выделен и не захвачен, обработка передается следующему элементу в списке
		// + в инкапсуляции состояния, и отсутствии необходимости извне опрашивать состояние кнопки
		if (!IsSelected())
			return false;

		if (IsSystemEnterEvent(event) && on_press)
			apply(on_press, args);
			//std::forward<ButtonArgSettingFunc>(on_press)(std::get<I>(std::forward<Tuple>(t))...);
			//on_press(args);

		return false;
	}
};

#endif
