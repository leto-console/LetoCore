/**
 * @file FileManagerMenu.hpp
 * @date Jul 25, 2026
 * @author Rakhimov T.
 */

#ifndef INC_FATFS_FILE_MANAGER_MENU_HPP
#define INC_FATFS_FILE_MANAGER_MENU_HPP

#include "LibrariesExport.h"

#include <SceneManager/ISceneBuilder.hpp>
#include <UI/Menu/ParamMenu.hpp>
#include <SDCard/SDCard_ExtDevice.hpp>

class LETO_CORE_EXPORT FileManager_Menu : public ParamMenu<uint8_t, 8>
{
	// param - flags
	enum : uint8_t
	{ 
		FLAG_LEVEL_UP   = (1 << 1),
		FLAG_DIR	    = (2 << 1),
		FLAG_FILE	    = (3 << 1),
		FLAG_GAME	    = (4 << 1),
        FLAG_CLOSE      = (5 << 1)
	};

    SDCard_ExtDevice* sdcard{};
    ExtDeviceStatus last_status = ExtDeviceStatus::UNDEFINED;

    ButtonCatcher<FileManager_Menu> left_level_up, left_close;

    void LevelUp();

    void Close();

	StaticList<StaticText32, 4> path;
	char char_path[128]{};
    bool refreshed{};

    bool UpdateCharPath();

    void AppendDir(const char* name);
	void AppendFile(const char* name);
	void AppendGame(const char* name);

    void RefreshLoop();

public:
	FileManager_Menu(Point2_i position);
	
    const char* GetCharPath() const { return char_path; }

    void InitDevice(SDCard_ExtDevice* device);

    void Reset();

	void Refresh();
    void SwitchItem(const StaticText32& name);
		
	void Enter() override;
	
	RGBColor GetColor(int idx) const override;

    bool ProcessInput(const AppEvent& event) override;
    void Loop() override;
};

#endif /* INC_FATFS_FILE_MANAGER_MENU_HPP */
