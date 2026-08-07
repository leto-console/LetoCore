/**
 * @file FileManager.hpp
 * @date Jul 25, 2026
 * @author Rakhimov T.
 */

#ifndef INC_FATFS_FILE_MANAGER_HPP
#define INC_FATFS_FILE_MANAGER_HPP

#include <SceneManager/ISceneBuilder.hpp>
#include <FatFs/FileManager_Menu.hpp>
#include <Input/SystemInputID.hpp>
#include <SceneManager/ISceneManager.hpp>
#include <UI/Text/UI_Label.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>
#include <SDCard/SDCard_ExtDevice.hpp>

class FileManager : public IScene
{
protected:
    UI_Label path;
	FileManager_Menu menu;

public:
	FileManager(ISceneManager* scene_manager) 
        : IScene{scene_manager}, menu{ Point2_i{ 0, 8 } }
	{
        path.SetPosition({80, 0});
        path.SetHorizonAlignment(LabelHorizonAlignment::CENTER);
        path.SetFont(IFont::FromHandle(leto_api_v1->Font->GetFont(7, 7, 1)));
        path.Enable();
        AddObject(&path);

        menu.InitDevice(sdcard_extdev);
        menu.InitBaseCatchers();
        menu.SetResetOnShow(false);
		menu.Enable();
		AddObject(&menu);
	}

    void OnShow() override
    {
        menu.Refresh();
        path.SetText(menu.GetCharPath());
    }

    void Loop() override
    {
        if (sdcard_extdev && 
            sdcard_extdev->GetStatus() == ExtDeviceStatus::READY)
        {
            path.SetText(menu.GetCharPath());
            path.SetTextColor(WhiteColor);
            path.SetColorInverse(false);
        }
        else
        {
            path.SetText("ОШИБКА SD-КАРТЫ!");
            path.SetTextColor(DeepOrangeColor);
            path.SetColorInverse(true);
        }

        if (!menu.IsActive())
            scene_manager->Return();
    }
	
	SCENE_NO_ARGS_BUILDER(FileManager)
};

#endif /* INC_FATFS_FILE_MANAGER_HPP */
