/**
 * @file EncoderCatcher.hpp
 * @date Jul 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_INPUT_CATCHER_ENCODER_CATCHER_HPP_
#define INC_INPUT_CATCHER_ENCODER_CATCHER_HPP_

#include <Input/Catchers/IEncoderCatcher.hpp>

template <typename Class>
class EncoderCatcher : public IEncoderCatcher
{    
public:
    using OnCatchFunc = void(Class::*)(bool);

    EncoderCatcher() = default;

    EncoderCatcher(Class* instance, OnCatchFunc callback)
    {
    	InitInstance(instance, callback);
    }

    void InitInstance(Class* instance, OnCatchFunc callback)
	{
		this->instance = instance;
		this->callback = callback;
        Enable();
	}

protected:
    Class* instance;
    OnCatchFunc callback{};

    void Callback(bool left) override
    {
        if (instance && callback)
            (instance->*callback)(left);
    }
};

#endif
