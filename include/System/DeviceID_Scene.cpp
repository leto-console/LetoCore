#include "DeviceID_Scene.hpp"

#include <UI/ValueSettingUI.hpp>
#include <UI/ButtonSettingUI.hpp>

#include <System/SystemRestart.hpp>

#include <Graphics/DefaultFont.hpp>
#include <SceneManager/ISceneManager.hpp>
#include <Auth/AuthScene.hpp>

void DeviceID_Scene::OnRandom()
{
}

void DeviceID_Scene::OnSave()
{
    SystemRestart();
}

DeviceID_Scene::DeviceID_Scene(ISceneManager *manager, AuthScene &auth_scene, IDataCell<uint32_t>* SerialNumber)
    : IScene{manager}, auth_scene{auth_scene}, settings{"", &manager->GetCommonAllocator()}
{
    settings.AddSetting<ValueEditableSettingUI<uint32_t>>("Ser.Num:", Point2_i{30, 44}, SerialNumber, "%d", 0, 10000, 1, false);

	//settings.AddSetting<ButtonCallInstanceSettingUI<DeviceID_Scene>>("Рандом", Point2_i{30, 47}, this, &DeviceID_Scene::OnRandom);
	settings.AddSetting<ButtonCallInstanceSettingUI<DeviceID_Scene>>("Перезагрузить", Point2_i{30, 60}, this, &DeviceID_Scene::OnSave);

	settings.SetFont(&Default_Font_7x7_small);

	settings.Enable();
	AddObject(&settings);
}

void DeviceID_Scene::Draw(IScreen &screen)
{
    leto_api_v1->Graphics->DrawRoundRect(IScreen::ToHandle(&screen), 1, 1, 160-2, 128-2, 10, 1, WhiteColor);
}
