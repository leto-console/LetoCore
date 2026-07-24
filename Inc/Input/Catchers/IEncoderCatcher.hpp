/**
 * @file IEncoderCatcher.hpp
 * @date Jul 24, 2026
 * @author Rakhimov T.
 */

#ifndef INC_INPUT_CATCHER_I_ENCODER_CATCHER_HPP_
#define INC_INPUT_CATCHER_I_ENCODER_CATCHER_HPP_

#include <cstdint>

#include <Input/EncoderEvent.hpp>

#include <SceneManager/ISceneObject.hpp>

enum EncoderCatcherMode : uint16_t
{
    ECM_ROTATE          = (1 << 0),    ///< Callback on rotate (L-R)
    ECM_STEP_HOLD       = (1 << 1),    ///< Callback on deviation from zero
};

class IEncoderCatcher : public ISceneObject
{    
public:
    IEncoderCatcher() = default;

    virtual void Callback(bool left) = 0;

    void Reset();

    // Отловить вывод требуемого типа
    void Catch(uint8_t _encoder_id, uint16_t _mode);

    void Draw(IScreen& screen, Point2_i offset = {}) { (void) screen; (void) offset; }

    void Loop() override;

    // Обработать событие пользовательского ввода
    bool ProcessInput(const AppEvent& event) override;

private:
    uint8_t encoder_id{};
    uint16_t mode{};
};

#endif
