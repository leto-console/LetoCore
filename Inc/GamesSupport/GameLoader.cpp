#include "GameLoader.hpp"

#include <System/CommonAllocator.hpp>
#include <LetoAPI_V1/Application/LetoApplication.hpp>
#include <LetoAPI_V1_System/Make.hpp>
#include <VirtualConsole/VirtualConsole.hpp>

// ======================================================================
//                    Независимый от платформы код
// ======================================================================

AppBinHeader* CurrentLoadedGame = nullptr;

// ======================================================================

// ======================================================================
//                        Реализация для Windows
// ======================================================================

#ifdef _WIN32

#include <Windows.h>
#include <cstring>

static HMODULE hLoadedDll;

uint32_t ScanGames(GameInfo *buffer, uint32_t available)
{
    WIN32_FIND_DATAA file_data;
    uint32_t count = 0;
    
    const char* base_path = "../../apps/win-debug/";
    const char* search_mask = "../../apps/win-debug/*.dll";

    HANDLE hFind = FindFirstFileA(search_mask, &file_data);
    
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        return 0;
    }

    do 
    {
        // Проверяем, что это файл, а не папка
        if (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
        {
            char rel_path[64];
            snprintf(rel_path, sizeof(rel_path), "%s%s", base_path, file_data.cFileName);

            if (CheckGame(rel_path, buffer[count]))
                count++;
        }
    } 
    while (count < available && FindNextFileA(hFind, &file_data));

    FindClose(hFind);
    return count;
}

bool GetBinary(const char *path, void* bin_info, uint32_t info_size, bool load_in_memory)
{	
	HMODULE hDll = LoadLibraryA(path);

	if (!hDll) return false;
	
    void *bin_ptr = reinterpret_cast<void*>(GetProcAddress(hDll, LETO_EXECUTABLE_INFO_VAR_TEXT));

	if (!bin_ptr || strncmp(reinterpret_cast<const char*>(bin_ptr), LETO_EXECUTABLE_TYPE_GAME, 16) != 0 )
	{
		FreeLibrary(hDll);
		return false;
	}

    memcpy(bin_info, bin_ptr, info_size);

    if (load_in_memory)
    {
        if (hLoadedDll) FreeLibrary(hLoadedDll);
        hLoadedDll = hDll;
    }
    else
        FreeLibrary(hDll);
	return true;
}

#endif

// ======================================================================

// ======================================================================
//                        Реализация для STM32
// ======================================================================

#ifdef USE_HAL_DRIVER

#include <FatFs/low_level/ff.h>
#include <HAL_include/HAL.hpp>

extern SPI_HandleTypeDef hspi1;

uint32_t ScanGames(GameInfo *buffer, uint32_t available)
{
	FRESULT res;
    uint32_t count = 0;

    DIR dir;
    res = f_opendir(&dir, "/");
    if(res != FR_OK)
    {
    	VC_Printf("[SG]f_opendir() failed, res = %d\r\n", RedColor, res);
        return 0;
    }

    FILINFO fileInfo;
    for(;count < available;)
    {
        res = f_readdir(&dir, &fileInfo);
        if((res != FR_OK) || (fileInfo.fname[0] == '\0'))
        {
            break;
        }

        if(!(fileInfo.fattrib & AM_DIR) && CheckGame(fileInfo.fname, buffer[count]))
        {
			VC_Printf("[SG] %s\r\n", BlueColor, buffer[count].en_name);
			count++;
        }
    }

    VC_Printf("[SG]Done!\r\n", BlueColor);
    return count;
}

constexpr uint32_t GRAM_BASE = 0x20016000;

bool GetBinary(const char *path, void* bin_info, uint32_t info_size, bool load_in_memory)
{
	static char static_buffer[256];

	char* buffer 			= load_in_memory ? (char*)GRAM_BASE	: static_buffer;
	uint32_t buffer_size 	= load_in_memory ? 40 * 1024 		: sizeof(static_buffer);

	FIL file;
	FRESULT res;
	UINT bytes_read;

	res = f_open(&file, path, FA_READ);
	if (res != FR_OK)
	{
		return false;
	}

	res = f_read(&file, buffer, buffer_size, &bytes_read);
	f_close(&file);

	if (res == FR_OK && bytes_read > 0 &&
		strncmp(buffer, LETO_EXECUTABLE_TYPE_GAME, 16) == 0)
	{
		memcpy(bin_info, buffer, info_size);
		return true;
	}
	return false;
}

#endif

bool CheckGame(const char *path, GameInfo &info)
{
	AppBinHeader bin_info{};

    if (!GetBinary(path, &bin_info, sizeof(AppBinHeader))) return false;

	info.api_version = bin_info.api_version;
	info.id = bin_info.id;
	snprintf(info.en_name, 		sizeof(info.en_name), 		bin_info.en_name);
	snprintf(info.ru_name, 		sizeof(info.ru_name), 		bin_info.ru_name);
	snprintf(info.path, 	    sizeof(info.path), 	        path);

	return true;
}

bool LoadGame(const char *path)
{
	VC_Printf("LoadGame\r\n", BlueColor);

    if (CurrentLoadedGame) return false;

    AppBinHeader bin_info{};
    if (!GetBinary(path, &bin_info, sizeof(AppBinHeader))) 
    {
        VC_Printf("Get AppBinHeader fail! \r\n", RedColor);
        return false;
    }
	VC_Printf("Get AppBinHeader success! \r\n", GreenColor);

    if (bin_info.api_version == 1)
    {
        LetoApplication_V1* app = CommonAllocator.Make<LetoApplication_V1>();
        if (!GetBinary(path, app, sizeof(LetoApplication_V1), true))
        {
        	VC_Printf("Get App_V1 fail \r\n", RedColor);
            return false;
        }
    	VC_Printf("Get App_V1 success!\r\n", GreenColor);

#ifdef USE_HAL_DRIVER
    	__DSB(); // Ждем, пока ВСЕ данные физически запишутся в RAM из буферов
    	__ISB(); // Очищаем конвейер, чтобы процессор гарантированно читал новые инструкции
#endif

        const LetoAPI_V1* api_v1 = Make_LetoAPI_V1();
        LetoResult_V1 res = app->Init(api_v1);
        if (res != LETO_V1_SUCCESS)
        {
        	VC_Printf("App_V1 Init fail\r\n", RedColor);
        	VC_Printf("%res=%d\r\n", RedColor, res);
            UnloadGame();
            return false;
        }
    	VC_Printf("App_V1 Init success!\r\n", GreenColor);

        CurrentLoadedGame = reinterpret_cast<AppBinHeader*>(app);
        return true;
    }

	VC_Printf("App_V%d not supported! \r\n", RedColor, bin_info.api_version);
    UnloadGame();
    return false;
}


void UnloadGame()
{
    if (CurrentLoadedGame)
    {
        if (CurrentLoadedGame->api_version == 1)
        {
            reinterpret_cast<LetoApplication_V1*>(CurrentLoadedGame)->Clean();
        }
        CommonAllocator.Clear(CurrentLoadedGame);
        CurrentLoadedGame = nullptr;
    }

#ifdef _WIN32
    if (hLoadedDll)
    {
        FreeLibrary(hLoadedDll);
        hLoadedDll = nullptr;
    }
#endif
}

// ======================================================================
