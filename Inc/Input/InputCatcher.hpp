/*
 * InputCatcher.hpp
 *
 *  Created on: Mar 15, 2026
 *      Author: Timur
 */

#ifndef INC_INPUT_INPUT_CATCHER_HPP_
#define INC_INPUT_INPUT_CATCHER_HPP_

#include <cstdint>

#include <Data/StaticList.hpp>
#include <ABI/AppEvent.hpp>
#include <Input/IsEventFunc.hpp>

template <typename Class>
class InputCatcher
{
public:
	using OnCatchFunc = void(Class::*)();
    using Comparator = bool(*)(const AppEvent& e1, const AppEvent& e2);
    
protected:
    StaticList<IsEventFunc, 4> is_events;

    Class* instance;
    OnCatchFunc callback{};
    Comparator comparator{};

public:
    InputCatcher(Comparator comparator) 
        : comparator{ comparator }
    {
    }

    InputCatcher(Comparator comparator, Class* instance, OnCatchFunc callback)
        : comparator{ comparator }
    {
        InitInstance(instance, callback);
    }

    void InitInstance(Class* instance, OnCatchFunc callback)
	{
		this->instance = instance;
		this->callback = callback;
	}

    // Отловить вывод требуемого типа
    void Catch(IsEventFunc is_event) { is_events.Push(is_event); }

    // Обработать событие пользовательского ввода
    bool ProcessInput(const AppEvent& event)
    {
        if (is_events.Count() == 0)
            return false;

        for (IsEventFunc _is_event : is_events)
        {
            /// TODO::::
            if (_is_event(event, true))
            {
                if (instance && callback)
                    (instance->*callback)();
                return true;
            }
        }
        
        return false;
    }
};

#endif
