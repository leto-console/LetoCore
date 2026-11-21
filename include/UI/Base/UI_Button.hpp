/**
 * @file UI_Button.hpp
 * @date Nov 20, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <type_traits>

#include <Data/Point2.hpp>
#include <Input/CatchersNew/ButtonCatcher.hpp>
#include <Input/SystemInputID.hpp>
#include <UI/Base/UI_Object.hpp>
#include <Utils/Caller.hpp>

#include <LetoAPI_V1/LetoAPI_V1.h>

NEW_LETO_UI_BEGIN

template <typename Derived>
class UI_Button : public Derived
{
    static_assert(std::is_base_of_v<UI_Object, Derived>, "UI_Object must be base of Derived");
public:
	UI_Button(ICaller& caller, Point2_i size = {}, const LetoAllocator_V1* allocator = nullptr) 
		: Derived{ size, allocator }, catcher{ this, &UI_Button::ButtonEvent }, caller{ caller }
	{
        UI_Object::AddChild(&catcher, true);
    }
	UI_Button(ICaller& caller, Point2_i size, UI_Object& parent)
		: Derived{ size, parent }, catcher{ this, &UI_Button::ButtonEvent }, caller{ caller }
	{
        UI_Object::AddChild(&catcher, true);
    }

    void InitBaseCatchers()
    {
        CatchButtonClick(SYSTEM_BTN_ENTER);
    }

    void CatchButtonClick(uint8_t button_id, uint16_t mode = BCM_SINGLE_PRESS | BCM_SINGLE_RELEASE)
    {
        catcher.Catch(button_id, mode);
    }

protected:
    ButtonCatcher<UI_Button> catcher;
    ICaller& caller;

    void ButtonEvent(uint16_t mode)
    {
        if (mode == BCM_SINGLE_PRESS)
        {
            UI_Object::Select(true);
        }
        else if (mode == BCM_SINGLE_RELEASE)
        {
            // Ужасная логика, подумай как исправить
            UI_Object::Select(false);
            caller.Call();
        }
    }
};

NEW_LETO_UI_END
