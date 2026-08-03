/*
 * FileConfig.cpp
 *
 *  Created on: Jan 30, 2025
 *      Author: Timur
 */

#ifndef USE_HAL_DRIVER

#include "Utils/FileConfig.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void FileConfig::SetConfigRootPath(std::string rootPath)
{ 
	filesystem_m::path potentialPath = rootPath;
	if (rootPath.empty())
	{
		// Инициализация папки с конфигами
		potentialPath = filesystem_m::current_path() / "ConfigIO";
	}
    if (!filesystem_m::exists(potentialPath))
    {
        filesystem_m::create_directory(potentialPath);
    }
	GetInstance().configRootPath = potentialPath;
}

void FileConfig::InitBlock(std::string blockName)
{
	filesystem_m::path blockPath = GetInstance().configRootPath / (blockName + ".txt");
    if (!filesystem_m::exists(blockPath))
    {
		std::ofstream file(blockPath);
		file.close();
    }
}

void FileConfig::InitString(std::string blockName, std::string paramName, std::string value)
{
	std::string temp;
	if (!ReadString(&temp, blockName, paramName))
	{
		InitBlock(blockName);
		std::fstream file(GetInstance().configRootPath / (blockName + ".txt"), std::fstream::out | std::fstream::app);
		if (!file.is_open() || !file.good())
		{
			file.close();
			return;
		}
		std::string line = (paramName + "=" + value + "\n");
		file << line;
		file.close();
	}
}

void FileConfig::InitFloat(std::string blockName, std::string paramName, float value)
{
	InitString(blockName, paramName, std::to_string(value));
}

void FileConfig::InitBool(std::string blockName, std::string paramName, bool value)
{
	InitString(blockName, paramName, value ? "1" : "0");
}

bool FileConfig::ReadString(std::string* ptr, std::string blockName, std::string paramName)
{
	bool found = false;
	if (GetInstance().configRootPath.empty())
		return found;
	std::ifstream file(GetInstance().configRootPath / (blockName + ".txt"));
	if (!file.is_open() || !file.good())
	{
		file.close();
		return false;
	}
	std::string key, value;
	while (std::getline(file, key, '='))
	{
		std::getline(file, value);
		if (key == paramName || paramName.empty())
		{
			*ptr = value;
			found = true;
			break;
		}
	}
	file.close();
	return found;
}

bool FileConfig::ReadFloat(float* ptr, std::string blockName, std::string paramName)
{
	std::string value;
	if (!ReadString(&value, blockName, paramName))
		return false;

	// Введено для обработки строковых float без учёта локали - всегда по "."
	std::istringstream istr(value.c_str());
	istr.imbue(std::locale("C"));
	istr >> *ptr;
	
	return true;
}

bool FileConfig::ReadFloat(SourceData<float>& data, size_t src, std::string blockName, std::string paramName)
{
	float temp;
	if (ReadFloat(&temp, blockName, paramName))
	{
		data.SetBySource(temp, src);
		return true;
	}
	return false;
}

bool FileConfig::ReadBool(bool* ptr, std::string blockName, std::string paramName)
{
	std::string value;
	if (!ReadString(&value, blockName, paramName))
		return false;
	for (char c : value)
	{
		if (c == '1')
		{
			*ptr = true;
			return true;
		}
		else if (c == '0')
		{
			*ptr = false;
			return true;
		}
	}
	return false;
}

void FileConfig::SetString(std::string blockName, std::string paramName, std::string paramValue)
{
	InitBlock(blockName);

	std::ifstream file_in(GetInstance().configRootPath / (blockName + ".txt"));
	if (paramName.empty() || !file_in.is_open() || !file_in.good())
	{
		file_in.close();
		return;
	}

	std::string key, value;
	std::stringstream text;
	bool found = false;
	while (std::getline(file_in, key, '='))
	{
		std::getline(file_in, value);
		if (key == paramName)
		{
			value = paramValue;
			found = true;
		}
		std::string line = (paramName + "=" + value + "\n");
		text << line;		
		if (found) break;
	}
	
	if (!found)
	{
		std::string line = (paramName + "=" + paramValue + "\n");
		text << line;	
	}
	file_in.close();

	std::ofstream file_out(GetInstance().configRootPath / (blockName + ".txt"));
	file_out << text.str();
	file_out.close();
}

void FileConfig::SetFloat(std::string blockName, std::string paramName, float paramValue)
{
	SetString(blockName, paramName, std::to_string(paramValue));
}

void FileConfig::SetBool(std::string blockName, std::string paramName, bool paramValue)
{
	SetString(blockName, paramName, paramValue == false ? "0" : "1");
}

#endif