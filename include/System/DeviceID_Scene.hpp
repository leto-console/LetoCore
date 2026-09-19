/**
 * @file DeviceID_Scene.hpp
 * @date Sep 19, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_DEVICE_ID_SCENE_HPP_
#define INC_SYSTEM_DEVICE_ID_SCENE_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <SceneManager/IScene.hpp>
#include <SceneManager/ISceneBuilder.hpp>

#include <Data/IDataCell.hpp>

#include <UI/SettingsContainer.hpp>

class AuthScene;

class LETO_CORE_EXPORT DeviceID_Scene : public IScene
{
protected:
    AuthScene& auth_scene;
	SettingsContainer settings;

    void OnRandom();
    void OnSave();

public:
    DeviceID_Scene(ISceneManager* manager, AuthScene& auth_scene, IDataCell<uint32_t>* SerialNumber);

    void Draw(IScreen& screen) override;
};

#endif
