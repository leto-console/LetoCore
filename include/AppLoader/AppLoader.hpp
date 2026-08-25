/**
 * @file AppLoader.hpp
 * @date May 26, 2026
 * @author Rakhimov T.
 */

#ifndef INC_APP_LOADER_APP_LOADER_HPP_
#define INC_APP_LOADER_APP_LOADER_HPP_

#include "LibrariesExport.h"

#include <LetoABI/AppBinHeader.h>

#include <AppLoader/AppInfo.hpp>
#include <Data/IAllocator.hpp>

/// @brief Текущая запущенная игра
extern LETO_CORE_EXPORT AppBinHeader* CurrentLoadedApp;

/**
 * @brief Получить содержимое исполняемого файла в бинарном виде
 * 
 * @param[in] path Путь до исполнямого файла
 * @param[out] bin_info Указатель на буфер для содержимого файла
 * @param[out] info_size Размер буфера
 * @param[in] load_in_memory Загружать ли бинарный файл в исполняемую часть памяти
 */
extern LETO_CORE_EXPORT bool GetBinary(const char *path, void* bin_info, uint32_t info_size, bool load_in_memory = false);

/**
 * @brief Просканировать файловую систему на наличие игр
 * 
 * @param[out] array Массив для записи информации об играх
 * @param[in] available Размер массива
 * @return Количество найденных приложений
 */
extern LETO_CORE_EXPORT uint32_t ScanApps(AppInfo* array, uint32_t available);

/**
 * @brief Проверить файл на соответствие исполняемому файлу игры
 * 
 * @param[in] path Путь до проверяемого файла
 * @param[out] info Информация об игре, в случае соответствия
 */
extern LETO_CORE_EXPORT bool CheckGame(const char* path, AppInfo& info);

/**
 * @brief Загрузить игру
 * 
 * @param[in] path Путь до исполнямого файла для загрузки 
 */
extern LETO_CORE_EXPORT bool LoadApplication(const char* path);

/**
 * @brief Выгрузить игру
 */
extern LETO_CORE_EXPORT void UnloadApplication();

#endif
