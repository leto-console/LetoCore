/**
 * @file IButtonCatcher.hpp
 * @date Jul 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_INPUT_CATCHER_I_BUTTON_CATCHER_HPP_
#define INC_INPUT_CATCHER_I_BUTTON_CATCHER_HPP_

#include "LetoCoreExport.h"

#include <cstdint>

#include <Data/StaticList.hpp>
#include <Time/Timer.hpp>
#include <UI/Base/UI_Object.hpp>

NEW_LETO_UI_BEGIN

enum ButtonCatcherMode : uint16_t
{
    BCM_SINGLE_PRESS     = (1 << 0),    ///< Callback on single press
    BCM_SINGLE_RELEASE   = (1 << 1),    ///< Callback on single release
    BCM_DOUBLE_CLICK     = (1 << 2),    ///< Callback on double click
    BCM_HOLD             = (1 << 3),    ///< Callback on hold
    BCM_HOLD_REPETITION  = (1 << 4),    ///< Callback on hold with repetition
    BCM_HOLD_MULTIPLY    = (1 << 5),    ///< Callback on hold multiply 
};

class LETO_CORE_EXPORT IButtonCatcher : public UI_Object
{    
public:
    IButtonCatcher() = default;

    virtual void Callback(uint16_t mode) = 0;

    void Reset();

    // Отловить вывод требуемого типа
    void Catch(uint8_t _button_id, uint16_t _mode);

    void SetHoldTime(uint32_t _hold_ms, uint32_t _multiply_ms = 0);

    void ObjectLoop() override;

    // Обработать событие пользовательского ввода
    bool ObjectOnEvent(const AppEvent& event) override;

private:
    StaticList<uint8_t, 4> button_id;
    bool button_pressed[4]{};

    uint16_t mode{};

    bool GetIdxByID(uint8_t& idx, uint32_t id) const;

    const uint32_t double_ms{ 200 };
    uint32_t hold_ms{}, multiply_ms{};
    Timer repeatition_timer;

    bool pressed{};
    bool holded{};
    bool multiplied{};
    uint32_t last_click_ms{};
};

NEW_LETO_UI_END

#endif
