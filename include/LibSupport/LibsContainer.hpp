/*
 *	LibsContainer.hpp
 *
 *	Created on: Mar 12, 2026
 *		Author: Timur
 */

#ifndef INC_LIBS_SUPPORT_LIB_CONTAINER_HPP_
#define INC_LIBS_SUPPORT_LIB_CONTAINER_HPP_

#include <type_traits>
#include <cstddef>

#include <Data/StaticList.hpp>

#include "../LibSupport/LibVersion.hpp"

// Результат поиска библиотеки
enum class LibFindResult : unsigned char
{ 
	LIB_NOT_FOUND, 		// Библиотека не найдена
	LIB_MAJOR_HIGH,		// Доступная версия слишком новая (запрашивается слишком старая)
	LIB_MAJOR_LOW, 		// Доступная версия слишком старая (запрашивается слишком новая)
	LIB_MINOR, 			// Доступная версия может не иметь требуемых функций (запрашивается обновленная актуальная библиотека)
	LIB_FOUND 			// Библиотека найдена
};

template <typename LibType, size_t MaxCount = 16>
class LibsContainer
{
	static_assert(offsetof(LibType, version) == 0);
	static_assert(std::is_standard_layout<LibType>::value);

protected:
	StaticList<LibType*, MaxCount> libs;

	bool CheckLibType(LibType* lib)
	{
		LibVersion cmp;
		return memcmp(lib->version.unique, cmp.unique, sizeof(cmp.unique)) == 0;
	}
	
public:
	void RegLib(LibType* lib)
	{
		if (!CheckLibType(lib)) return;
		libs.Push(lib);
	}
		
	LibFindResult Find(LibVersion version, LibType*& lib)
	{
		LibFindResult res = LibFindResult::LIB_NOT_FOUND;
		
		for (LibType* _lib : libs)
		{
			if (_lib->version.major > version.major)
			{
				// Игры написана на устаревшей библиотеке. Данная игра не поддерживается на текущей платформе
				if (res < LibFindResult::LIB_MAJOR_HIGH)
					res = LibFindResult::LIB_MAJOR_HIGH;
			}
			else if (_lib->version.major < version.major)
			{
				// Игра предназначена для более новой системы. Обновите систему
				if (res < LibFindResult::LIB_MAJOR_LOW)
					res = LibFindResult::LIB_MAJOR_LOW;
			}
			else if (_lib->version.minor < version.minor)
			{
				// Игре могут потребоваться функции обновленной системы. Обновите систему
				if (res < LibFindResult::LIB_MINOR)
					res = LibFindResult::LIB_MINOR;
			}
			else if (CheckLibType(_lib))
			{
				// Для нахождения лучшей библиотеки (version - локальный, так что так можно)
				version.minor = _lib->version.minor; 	
				lib = _lib;
				res = LibFindResult::LIB_FOUND;
			}
		}

		return res;
	}
};

#endif