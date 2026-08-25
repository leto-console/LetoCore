/**
 * @file AppInfo.hpp
 * @date May 25, 2026
 * @author Rakhimov T.
 */

#ifndef INC_APP_LOADER_APP_INFO_HPP_
#define INC_APP_LOADER_APP_INFO_HPP_

#include "LibrariesExport.h"

#include <cstdint>
#include <type_traits>

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
	/// TODO(Rakhimov T.): Increase buffer size (e.g., to 128 or 256) for PC
	char path[64] {};
};

static_assert(std::is_standard_layout<AppInfo>::value, "AppInfo must be a standard layout type");

#endif
