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

    ButtonCatcher() = default;

    ButtonCatcher(Class* instance, OnCatchFunc callback)
    {
    	InitInstance(instance, callback);
    }

    void InitInstance(Class* instance, OnCatchFunc callback)
	{
		this->instance = instance;
		this->callback = callback;
        Enable(true);
	}

protected:
    Class* instance;
    OnCatchFunc callback{};

    void Callback() override final
    {
        if (instance && callback)
            (instance->*callback)();
    }
};

NEW_LETO_UI_END

#endif
