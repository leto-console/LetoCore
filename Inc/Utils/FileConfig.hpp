/*
 * FileConfig.hpp
 *
 *  Created on: Jan 30, 2025
 *      Author: Timur
 */

#ifndef USE_HAL_DRIVER

#ifndef INC_UTILS_FILECONFIG_HPP_
#define INC_UTILS_FILECONFIG_HPP_

#include "LibrariesExport.h"

#include <string>
#include "Data/SourceData.hpp"

#if __cplusplus >= 201703L
#include <filesystem>
#define filesystem_m std::filesystem
#else
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#define filesystem_m std::experimental::filesystem
#endif

/*
	Класс, реализующий доступ к конфигурационным файлам, блокам и данным
*/
class LETO_CORE_EXPORT FileConfig
{
protected:
	FileConfig() {};
	filesystem_m::path configRootPath = "";

public:
	static FileConfig& GetInstance()
	{
		static FileConfig instance;
		return instance;
	}

	/* 
		Установить путь к корню папки с конфигурационными файлами.
		При вызове функции с параметром по умолчанию будет создана
		папка ConfigIO в корне, где располагается файл FileConfig.cpp
	*/
	static void SetConfigRootPath(std::string rootPath = "");

	/* Создать, если не существует, блок с названием blockName */
	static void InitBlock(std::string blockName);
	/*  
		Создать, если не существует, блок с названием blockName и,
		если параметра paramName не существует, создать его и задать
		ему стандартное значение = value
	*/
	static void InitString(std::string blockName, std::string paramName, std::string value = "");
	static void InitFloat(std::string blockName, std::string paramName, float value = 0.0);
	static void InitBool(std::string blockName, std::string paramName, bool value = false);

	/* 
		Прочитать из блока с названием blockName параметр paramName типа string.
		В случае успеха функция возвращает true и присваивает переменной *ptr значение,
		в противном случае возвращает false и оставляет переменную *ptr нетронутой
	*/
	static bool ReadString(std::string* ptr, std::string blockName, std::string paramName);
	static bool ReadFloat(float* ptr, std::string blockName, std::string paramName);
	static bool ReadFloat(SourceData<float>& data, size_t src, std::string blockName, std::string paramName);
	static bool ReadBool(bool* ptr, std::string blockName, std::string paramName);

	/* 
		Записать в блок с названием blockName параметр paramName со значением paramValue.
		Если блока не существует, он будет создан. Если параметр существует, он будет перезаписан
	*/
	static void SetString(std::string blockName, std::string paramName, std::string paramValue);
	static void SetFloat(std::string blockName, std::string paramName, float paramValue);
	static void SetBool(std::string blockName, std::string paramName, bool paramValue);

	FileConfig(FileConfig const&) = delete;
	void operator=(FileConfig const&) = delete;
};

#endif /* INC_UTILS_FILECONFIG_HPP_ */

#endif /* USE_HAL_DRIVER */