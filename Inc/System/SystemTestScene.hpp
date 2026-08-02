/**
 * @file SystemTest.hpp
 * @date Jul 02, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_SYSTEM_TEST_HPP_
#define INC_SYSTEM_SYSTEM_TEST_HPP_

#include "LibrariesExport.h"

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>

class LETO_CORE_EXPORT SystemTestScene : public IScene
{
public:
    SystemTestScene();

    void Draw(IScreen& screen) override;

    bool Loop() override;

    bool ProcessInput(const AppEvent& event) override;

    SCENE_NO_ARGS_BUILDER(SystemTestScene)

protected:
    enum { 
        STAGE_NONE, 
        STAGE_COLOR_R,
        STAGE_COLOR_G,
        STAGE_COLOR_B,
        STAGE_COLOR_PALLETE,
        STAGE_COLOR_GRAY,
        //STAGE_INPUT
        STAGE_DONE,
    };

    uint8_t stage = STAGE_NONE;
};

#endif
