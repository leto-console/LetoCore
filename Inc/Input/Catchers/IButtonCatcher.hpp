/**
 * @file IButtonCatcher.hpp
 * @date Jul 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_INPUT_CATCHER_I_BUTTON_CATCHER_HPP_
#define INC_INPUT_CATCHER_I_BUTTON_CATCHER_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <SceneManager/ISceneObject.hpp>
#include <Time/Timer.hpp>

enum ButtonCatcherMode : uint16_t
{
    BCM_SINGLE_PRESS     = (1 << 0),    ///< Callback on single press
    BCM_SINGLE_RELEASE   = (1 << 1),    ///< Callback on single release
    BCM_DOUBLE_CLICK     = (1 << 2),    ///< Callback on double click
    BCM_HOLD             = (1 << 3),    ///< Callback on hold
    BCM_MULTI_HOLD       = (1 << 4),    ///< Callback on hold multiply 
};

class LIBRARIES_EXPORT IButtonCatcher : public ISceneObject
{    
public:
    IButtonCatcher() = default;

    virtual void Callback() = 0;

    // Отловить вывод требуемого типа
    void Catch(uint8_t _button_id, uint16_t _mode);

    void SetHoldTime(uint32_t _hold_ms, uint32_t _multiply_ms = 0);

    void Draw(IScreen& screen, Point2_i offset = {}) { (void) screen; (void) offset; }

    void Loop() override;

    // Обработать событие пользовательского ввода
    bool ProcessInput(const AppEvent& event) override;

private:
    uint8_t button_id{};
    uint16_t mode{};
    bool pressed{};
    bool holded{};
    uint32_t last_click_ms{};
    
    const uint32_t double_ms{ 100 };
    uint32_t hold_ms{}, multiply_ms{};
    Timer multiply_timer;
};

#endif
