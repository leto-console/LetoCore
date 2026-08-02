#ifndef INC_UTILS_ARG_PARSER_HPP
#define INC_UTILS_ARG_PARSER_HPP

#include "LibrariesExport.h"

#include <stdint.h>
#include <string>
#include <unordered_map>

/*
	Парсер аргументов командной строки формата:
	--arg value -a value
*/
class LETO_CORE_EXPORT ArgParser
{
protected:
	std::unordered_map<std::string, std::string> key_value;

public:
	ArgParser(int argc, char** argv);

	// Найти аргумент
	bool FindArg(const std::string& arg, std::string* value = nullptr);
	
	// Найти значение при ключе или вернуть значение по умолчанию
	std::string FindValue(const std::string& arg, const std::string& default_value);
};

#endif 
