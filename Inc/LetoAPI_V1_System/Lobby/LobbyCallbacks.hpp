/**
 * @file LobbyCallbacks.hpp
 * @date Jun 11, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_CALLBACKS_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_CALLBACKS_HPP_

#include <cstdint>

//#ifdef STM32F4_LIBRARIES_EXPORT     ///< These functions can only be used within Libraries


/**
 * @brief Callback function for handling messages from information port
 */
extern void LobbyInfoCallback(uint8_t /*channel*/, uint8_t /*port*/, uint32_t id, const void* data, uint32_t size);

/**
 * @brief Callback function for handling messages from main port
 */
extern void LobbyMainCallback(uint8_t /*channel*/, uint8_t /*port*/, uint32_t id, const void* data, uint32_t size);


//#endif /* STM32F4_LIBRARIES_EXPORT */

#endif /* INC_LETO_API_V1_LOBBY_LOBBY_CALLBACKS_HPP_ */
