/**
 * @file GameLoader.hpp
 * @date May 26, 2026
 * @author Rakhimov T.
 */

#ifndef INC_GAME_SUPPORT_GAME_LOADER_HPP_
#define INC_GAME_SUPPORT_GAME_LOADER_HPP_

#include "LibrariesExport.h"

#include <ABI/AppBinHeader.hpp>

#include <GamesSupport/BaseGame.hpp>
#include <GamesSupport/GameInfo.hpp>

/// @brief Текущая запущенная игра
extern LIBRARIES_EXPORT AppBinHeader* CurrentLoadedGame;

/**
 * @brief Получить содержимое исполняемого файла в бинарном виде
 * 
 * @param[in] path Путь до исполнямого файла
 * @param[out] bin_info Указатель на буфер для содержимого файла
 * @param[out] info_size Размер буфера
 * @param[in] load_in_memory Загружать ли бинарный файл в исполняемую часть памяти
 */
extern LIBRARIES_EXPORT bool GetBinary(const char *path, void* bin_info, uint32_t info_size, bool load_in_memory = false);

/**
 * @brief Просканировать файловую систему на наличие игр
 * 
 * @param[out] buffer Буфер для записи информации об играх
 * @param[in] available Размер буфера
 * @return Количество найденных игр
 */
extern LIBRARIES_EXPORT uint32_t ScanGames(GameInfo* buffer, uint32_t available);

/**
 * @brief Проверить файл на соответствие исполняемому файлу игры
 * 
 * @param[in] path Путь до проверяемого файла
 * @param[out] info Информация об игре, в случае соответствия
 */
extern LIBRARIES_EXPORT bool CheckGame(const char* path, GameInfo& info);

/**
 * @brief Загрузить игру
 * 
 * @param[in] path Путь до исполнямого файла для загрузки 
 */
extern LIBRARIES_EXPORT bool LoadGame(const char* path);

/**
 * @brief Выгрузить игру
 */
extern LIBRARIES_EXPORT void UnloadGame();

#endif
