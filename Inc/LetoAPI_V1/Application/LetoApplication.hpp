/**
 * @file LetoApplication.hpp
 * @date May 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_APPLICATION_BASE_APPLICATION_ABI_HPP_
#define INC_APPLICATION_BASE_APPLICATION_ABI_HPP_

#include <cstdint>

#include <ABI/AppBinHeader.hpp>
#include <ABI/AppEvent.hpp>
#include <LetoAPI_V1/LetoAPI_V1.hpp>
#include <LetoAPI_V1/Application/LetoAppStatus.hpp>
#include <LetoAPI_V1/Application/LetoResult.hpp>

#include <Graphics/IScreen.hpp>

#pragma pack(push, 4)

/**
 * @brief Leto OS application interface
 */
struct LetoApplication_V1
{
	/// Executable application header
	AppBinHeader Header;

	/**
	 * @brief Initialize application
	 */
	LetoResult_V1 (*Init) (const LetoAPI_V1* api);
	
	/**
	 * @brief Clean up and release application resources
	 */
	LetoResult_V1 (*Clean) ();

	/**
	 * @brief Get current application status
	 */
	LetoAppStatus_V1 (*GetStatus) ();
	
	/**
	 * @brief Process incoming event
	 *
	 * @return 
	 * - `APP_SUCCESS` if event was successfully handled
	 * 
	 * - `APP_UNSUPPORT` if event is not supported by application
	 */
	LetoResult_V1 (*ProcessEvent) (AppEvent event);
	
	/**
	 * @brief Render application UI
	 */
	LetoResult_V1 (*Draw) (IScreen* screen);
	
	/**
	 * @brief Execute background tasks or main loop iteration
	 */
	LetoResult_V1 (*Loop) ();
	
};

#pragma pack(pop)

static_assert(std::is_standard_layout<LetoApplication_V1>::value, "LetoApplication_V1 must be a standard layout type");

#endif
