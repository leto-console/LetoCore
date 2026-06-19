/**
 * @file LetoResult.hpp
 * @date May 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_APPLICATION_LETO_RESULT_HPP_
#define INC_LETO_API_V1_APPLICATION_LETO_RESULT_HPP_

#include <cstdint>

/// @brief Result of application interaction
using LetoResult_V1 = int32_t;

constexpr LetoResult_V1 LETO_V1_SUCCESS 		=  0;	///< Operation completed successfully
constexpr LetoResult_V1 LETO_V1_ERROR 			= -1;	///< Generic error
constexpr LetoResult_V1 LETO_V1_UNSUPPORT 		= -2;	///< Not supported
constexpr LetoResult_V1 LETO_V1_ARGS_ERROR		= -3;	///< Invalid arguments
constexpr LetoResult_V1 LETO_V1_ALREADY_DONE 	= -4;	///< Already executed previously
constexpr LetoResult_V1 LETO_V1_OUT_OF_MEMORY	= -5;	///< Out of dynamic memory

#endif
