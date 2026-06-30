/**
 * @file LobbyAPI_V1.hpp
 * @date Jun 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_API_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_API_HPP_

#include <cstdint>
#include <type_traits>

#include <LetoAPI_V1/Lobby/LobbyConnection.hpp>
#include <LetoAPI_V1/Lobby/LobbyInfo.hpp>

#pragma pack(push, 4)

/// Game lobby management functions
struct LobbyAPI_V1
{
    /**
	 * @brief Get list of available game lobbies nearby
	 * 
	 * @param[out] info Array to receive information
	 * @param[in] available Size of the destination array
	 *
	 * @return Number of game lobbies
	 */
	uint32_t (*const GetLobbiesNear) (LobbyInfo_V1* info, uint32_t available);

    /**
     * @brief Create lobby
     * 
     * @param[out] lobby Opened lobby connection instance
     * @param[in] app_id Application identifier
     * @param[in] max_count Maximum number of connections
     */
    bool (*const CreateLobby)(LobbyConnection_V1* lobby, uint16_t app_id, uint8_t max_count, LobbyConnection_V1_Callback callback);

    /**
     * @brief Join lobby
     * 
     * @warning Connection is established asynchronously. The outcome can be checked via `GetActiveLobby`.
     * 
     * @param[in] info Lobby information
     * @param[in] callback Callback function to receive data from the lobby
     */
    void (*const JoinLobby)(LobbyInfo_V1 info, LobbyConnection_V1_Callback callback);

    /**
     * @brief Check for active lobby connection
     * 
     * @param[out] lobby Active lobby connection details
     * 
     * @return `true` if successfully connected
     */
    bool (*const GetActiveLobby)(LobbyConnection_V1* lobby);

    /**
     * @brief Disconnect from active lobby
     */
    void (*const QuitLobby)();

    /**
     * @brief Disconnect specific member from lobby
     * 
     * @param[in] id Lobby member identifier
     */
    void (*const DisconnectMember)(uint32_t id);

    /**
	 * @brief Send unreliable data to active lobby
	 *
	 * @param[in] data Data to send
	 * @param[in] size Size of data to send
	 */
	void (*const SendData)(const void* data, uint32_t size);

    /**
     * @brief Set readiness for connection.
     * 
     * @param[in] ready True if ready for connection, false otherwise.
     */
    void (*const SetReady)(bool ready);

    /**
     * @brief Get readiness for connection.
     * 
     * @return True if ready for connection, false otherwise.
     */
    bool (*const GetReady)();
};

#pragma pack(pop)

static_assert(std::is_standard_layout<LobbyAPI_V1>::value, "LobbyAPI_V1 must be a standard layout type");

#endif
