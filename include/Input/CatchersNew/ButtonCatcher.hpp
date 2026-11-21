/**
 * @file ButtonCatcher.hpp
 * @date Jul 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_INPUT_CATCHER_BUTTON_CATCHER_HPP_
#define INC_INPUT_CATCHER_BUTTON_CATCHER_HPP_

#include <Input/CatchersNew/IButtonCatcher.hpp>

NEW_LETO_UI_BEGIN

template <typename Class>
class ButtonCatcher : public IButtonCatcher
{    
public:
    using OnCatchFunc = void(Class::*)();
    using OnCatchModeFunc = void(Class::*)(uint16_t);

    ButtonCatcher() = default;

    ButtonCatcher(Class* instance, OnCatchFunc callback)
    {
    	InitInstance(instance, callback);
    }

    ButtonCatcher(Class* instance, OnCatchModeFunc callback)
    {
    	InitInstance(instance, callback);
    }

    void InitInstance(Class* instance, OnCatchFunc callback)
	{
		this->instance = instance;
		this->callback = callback;
        Enable(true);
	}

    void InitInstance(Class* instance, OnCatchModeFunc callback)
	{
		this->instance = instance;
		this->callback_mode = callback;
        Enable(true);
	}
protected:
    Class* instance;
    OnCatchFunc callback{};
    OnCatchModeFunc callback_mode{};

    void Callback(uint16_t mode) override final
    {
        if (instance)
        {
            if (callback) (instance->*callback)();
            if (callback_mode) (instance->*callback_mode)(mode);
        }
            
    }
};

NEW_LETO_UI_END

#endif
