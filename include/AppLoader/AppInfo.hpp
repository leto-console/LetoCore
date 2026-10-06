/**
 * @file AppInfo.hpp
 * @date May 25, 2026
 * @author Rakhimov T.
 */

#ifndef INC_APP_LOADER_APP_INFO_HPP_
#define INC_APP_LOADER_APP_INFO_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <cstdio>
#include <type_traits>

#include <LetoABI/AppBinHeader.h>

/**
 * @brief Application information used for temporary storage
 */
struct AppInfo
{	
	/// API major version used by the application
	uint16_t api_version;

	/// Unique application identifier
	uint16_t id;
		
	/// Application name in English
	char en_name[32] {};
	
	/// Application name in Russian
	char ru_name[32] {};
	
	/// Path to the executable file
#ifdef __STM32__
	char path[128] {};
#else
	char path[260] {};
#endif

	void FromBinary(AppBinHeader& bin_info)
	{
		api_version = bin_info.api_version;
		id = bin_info.id;
		snprintf(en_name, sizeof(en_name), bin_info.en_name);
		snprintf(ru_name, sizeof(ru_name), bin_info.ru_name);
	}
};

static_assert(std::is_standard_layout<AppInfo>::value, "AppInfo must be a standard layout type");

#endif
