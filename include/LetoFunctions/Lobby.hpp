/**
 * @file Lobby.hpp
 * @date Oct 16, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>
#include <LetoFunctions/CheckAPI.hpp>

namespace leto::lobby
{

        /**
	 * @brief Get list of available game lobbies nearby
	 * 
	 * @param[out] info Array to receive information
	 * @param[in] available Size of the destination array
	 *
	 * @return Number of game lobbies
	 */
	inline uint32_t GetLobbiesNear (struct LobbyInfo_V1* info, uint32_t available)
    {
        LETO_CHECK_API(leto_api_v1, Lobby, GetLobbiesNear, 0);
        return leto_api_v1->Lobby->GetLobbiesNear(info, available);
    }

    /**
     * @brief Create lobby
     * 
     * @param[out] lobby Opened lobby connection instance
     * @param[in] app_id Application identifier
     * @param[in] max_count Maximum number of connections
     * @param[in] callback Callback function to receive data from the lobby
     */
    inline bool CreateLobby(struct LobbyConnection_V1* lobby, uint16_t app_id, uint8_t max_count, LobbyConnection_V1_Callback callback)
    {
        LETO_CHECK_API(leto_api_v1, Lobby, CreateLobby, false);
        return leto_api_v1->Lobby->CreateLobby(lobby, app_id, max_count, callback);
    }

    /**
     * @brief Join lobby
     * 
     * @warning Connection is established asynchronously. The outcome can be checked via `GetActiveLobby`.
     * 
     * @param[in] info Lobby information
     * @param[in] callback Callback function to receive data from the lobby
     */
    inline void JoinLobby(struct LobbyInfo_V1 info, LobbyConnection_V1_Callback callback)
    {
        LETO_CHECK_API(leto_api_v1, Lobby, JoinLobby,);
        leto_api_v1->Lobby->JoinLobby(info, callback);
    }

    /**
     * @brief Check for active lobby connection
     * 
     * @param[out] lobby Active lobby connection details
     * 
     * @return `true` if successfully connected
     */
    inline bool GetActiveLobby(struct LobbyConnection_V1* lobby)
    {
        LETO_CHECK_API(leto_api_v1, Lobby, GetActiveLobby, false);
        return leto_api_v1->Lobby->GetActiveLobby(lobby);
    }

    /**
     * @brief Disconnect from active lobby
     */
    inline void QuitLobby()
    {
        LETO_CHECK_API(leto_api_v1, Lobby, QuitLobby,);
        leto_api_v1->Lobby->QuitLobby();
    }

    /**
     * @brief Disconnect specific member from lobby
     * 
     * @param[in] id Lobby member identifier
     */
    inline void DisconnectMember(uint32_t id)
    {
        LETO_CHECK_API(leto_api_v1, Lobby, DisconnectMember,);
        leto_api_v1->Lobby->DisconnectMember(id);
    }

    /**
	 * @brief Send unreliable data to active lobby
	 *
	 * @param[in] data Data to send
	 * @param[in] size Size of data to send
	 */
	inline void SendData(const void* data, uint32_t size)
    {
        LETO_CHECK_API(leto_api_v1, Lobby, SendData,);
        leto_api_v1->Lobby->SendData(data, size);
    }

    /**
     * @brief Set readiness for connection.
     * 
     * @param[in] ready True if ready for connection, false otherwise.
     */
    inline void SetReady(bool ready)
    {
        LETO_CHECK_API(leto_api_v1, Lobby, SetReady,);
        leto_api_v1->Lobby->SetReady(ready);
    }

    /**
     * @brief Get readiness for connection.
     * 
     * @return True if ready for connection, false otherwise.
     */
    inline bool GetReady()
    {
        LETO_CHECK_API(leto_api_v1, Lobby, GetReady, false);
        return leto_api_v1->Lobby->GetReady();
    }

}