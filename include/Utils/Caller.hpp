/**
 * @file Caller.hpp
 * @date Nov 21, 2026
 * @author Rakhimov T.
 */

#pragma once

struct ICaller
{
    virtual ~ICaller() = default;
    virtual void Call() = 0;
};

class CallbackCaller : public ICaller
{
public:
	using callback_t = void(*)();

protected:
	callback_t on_press{ nullptr };

public:
	explicit CallbackCaller(callback_t on_press) : on_press{ on_press }
    { }

	void Call() override
    {
        if (on_press) on_press();
    }
};

template <typename Instance>
class InstanceCaller : public ICaller
{
public:
	using callback_t = void(Instance::*)();

protected:
	Instance* instance{ nullptr };
	callback_t on_press{ nullptr };

public:
	InstanceCaller(Instance* instance, callback_t on_press) 
        : instance{ instance }, on_press{ on_press }
    { }

	void Call() override
    {
        if (instance && on_press)
			(instance->*on_press)();
    }
};
