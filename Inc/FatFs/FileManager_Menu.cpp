#include "FileManager_Menu.hpp"

#include <FatFs/low_level/ff.h>
#include <GamesSupport/GameLoader.hpp>
#include <LetoAPI_V1/LetoAPI_V1.hpp>
#include <Input/SystemInputID.hpp>

FileManager_Menu::FileManager_Menu(Point2_i position)
    : ParamMenu<uint8_t, 8>{ 8, position, true },
    left_level_up{ this, &FileManager_Menu::LevelUp },
    left_close{ this, &FileManager_Menu::Close }
{
    SetStyle(MenuStyle::STYLE_2, leto_api_v1->Font->GetFont(7, 7, 1));

    left_level_up.Catch(SYSTEM_BTN_LEFT, BCM_SINGLE_PRESS);
    left_level_up.Enable();
    
    left_close.Catch(SYSTEM_BTN_LEFT, BCM_HOLD);
    left_close.SetHoldTime(1500);
    left_close.Enable();
}

void FileManager_Menu::AppendDir(const char *name)
{
	AppendMenuItem(name, FLAG_DIR);
}

void FileManager_Menu::AppendFile(const char *name)
{
	AppendMenuItem(name, FLAG_FILE);
}

void FileManager_Menu::AppendGame(const char *name)
{
	AppendMenuItem(name, FLAG_GAME);
}

void FileManager_Menu::RefreshLoop()
{
    if (refreshed) return;

    bool ready = !sdcard || sdcard->GetStatus() == ExtDeviceStatus::READY;

    if (!ready)
        Reset();
    else
        Clear();
    
    if (UpdateCharPath() && path.Count() > 0)
        AppendMenuItem("..", FLAG_LEVEL_UP);
    else
        AppendMenuItem("...", FLAG_CLOSE);

    if (!ready) return;
    
    FRESULT res;
    DIR dir;
    res = f_opendir(&dir, char_path);
    if(res != FR_OK)
    {
        //VC_Printf("[SG]f_opendir() failed, res = %d\r\n", RedColor, res);
        return;
    }

    FILINFO fileInfo;
    GameInfo dummy;
    for(;;)
    {
        res = f_readdir(&dir, &fileInfo);
        if((res != FR_OK) || (fileInfo.fname[0] == '\0'))
        {
            break;
        }
        
        if (fileInfo.fattrib & AM_DIR)
            AppendDir(fileInfo.fname);
        else if (CheckGame(fileInfo.fname, dummy))
            AppendGame(fileInfo.fname);
        else
            AppendFile(fileInfo.fname);
    }

    SetCurrentID(1);
    refreshed = true;
}

void FileManager_Menu::InitDevice(SDCard_ExtDevice* device)
{
    sdcard = device;
}

void FileManager_Menu::Reset()
{
    path.Clear();
    Clear();
}

void FileManager_Menu::Refresh()
{
    refreshed = false;
}

void FileManager_Menu::SwitchItem(const StaticText32& name)
{
    for (int idx = 0; idx < Count(); ++idx)
    {
        if (name == texts[idx])
        {
            SetCurrentID(idx);
            return;
        }
    }
}

void FileManager_Menu::Enter()
{
    switch (GetCurrentParam())
    {
    case FLAG_LEVEL_UP:
        LevelUp();
        break;
    case FLAG_DIR:
        path.Push(GetCurrentText());
        Refresh();
        break;
    case FLAG_FILE:
        //Short view of the file
        break;
    case FLAG_GAME:
        //LoadGame or ShowInfo
        break;
    case FLAG_CLOSE:
        Close();
        break;
    default:
        break;
    }
}

RGBColor FileManager_Menu::GetColor(int idx) const
{
    switch (GetParam(idx))
    {
    case FLAG_DIR:
        return BlueColor;
    case FLAG_FILE:
        return YellowColor;
    case FLAG_GAME:
        return aRedColor;
    // case FLAG_LEVEL_UP:
    // case FLAG_CLOSE:
    //     return GrayColor;
    default:
        break;
    }
    return WhiteColor;
}

bool FileManager_Menu::ProcessInput(const AppEvent &event)
{
    if (ParamMenu<uint8_t, 8>::ProcessInput(event))
        return true;
    
    if (left_close.MainProcessInput(event) + 
        left_level_up.MainProcessInput(event))
        return true;

    return false;
}

void FileManager_Menu::Loop()
{
    ParamMenu<uint8_t, 8>::Loop();
    left_close.MainLoop();
    left_level_up.MainLoop();

    if (sdcard)
    {
        ExtDeviceStatus status = sdcard->GetStatus();
        if (last_status != status)
            Refresh();
        last_status = status;
    }

    RefreshLoop();
}

void FileManager_Menu::LevelUp()
{
    if (!path.Empty())
    {
        StaticText32 cur = path.Back();
        path.Pop();
        Refresh();
        SwitchItem(cur);
    }
}

void FileManager_Menu::Close()
{
    Disable();
}

bool FileManager_Menu::UpdateCharPath()
{
    uint32_t size = sizeof(char_path);
    if (size < 2) return false;
    char_path[0] = '/';
    char_path[1] = 0;
    
    char* edit_path = char_path;
    for (const StaticText32& text : path)
    {
        int res = snprintf(edit_path, size, "/%s", text.ConstChar());
        if (res < 0 || res > size)
            return false;
        edit_path += res;
        size -= res;
    }
    
    return true;
}
