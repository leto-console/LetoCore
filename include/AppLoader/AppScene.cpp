#include <AppLoader/AppScene.hpp>

#include <Input/SystemInputID.hpp>

#include <AppLoader/AppLoader.hpp>
#include <SceneManager/ISceneManager.hpp>

#include <LetoAPI_V1/Application/LetoApplication_V1.h>

StaticText<128> AppPath;

static uint8_t GameButtonIds[] = {
    SYSTEM_BTN_UP, SYSTEM_BTN_DOWN, SYSTEM_BTN_LEFT, SYSTEM_BTN_RIGHT, SYSTEM_BTN_ENTER
};

bool AppScene::IsInputForGame(const AppEvent &event)
{
	if (event.source == AE_Button)
	{
		for (uint8_t id : GameButtonIds)
		{
			if (event.id == id) 
				return true;
		}
		return false;
	}
	else if (event.source == AE_Encoder)
	{
		return event.id == SYSTEM_ENC_MAIN;
	}

	return false;
}

AppScene::AppScene(ISceneManager* scene_manager)
    : IScene{ scene_manager }
{
}

void AppScene::OnShow()
{
    LoadApplication(AppPath);
}

void AppScene::OnHide()
{
    UnloadApplication();
}

void AppScene::Draw(IScreen& screen)
{
    if (!CurrentLoadedApp) 
        return;
    
    if (CurrentLoadedApp->api_version == 1)
        reinterpret_cast<LetoApplication_V1*>(CurrentLoadedApp)->Draw(IScreen::ToHandle(&screen));
}

void AppScene::Loop()
{
	if (!CurrentLoadedApp)
    {
        scene_manager->Return();
        return;
    }

    if (CurrentLoadedApp->api_version == 1)
    {
        LetoApplication_V1* app = reinterpret_cast<LetoApplication_V1*>(CurrentLoadedApp);
        app->Loop();
        if (app->GetStatus() != LETO_V1_WORK_STATUS)
        {
            // ... + некоторые действия при завершении игры
            UnloadApplication();
            scene_manager->Return();
        }
    }
}

bool AppScene::ProcessInput(const AppEvent &event)
{
    if (!CurrentLoadedApp || !IsInputForGame(event))
        return true;

    // Think about switch-case
    if (CurrentLoadedApp->api_version == 1)
    {
        reinterpret_cast<LetoApplication_V1*>(CurrentLoadedApp)->ProcessEvent(event);
    }

    return true;
}