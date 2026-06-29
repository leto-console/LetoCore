/**
 * @file LobbyManager.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_LOBBY_LOBBY_MANAGER_HPP_
#define INC_LETO_API_V1_LOBBY_LOBBY_MANAGER_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <LetoAPI_V1/Lobby/LobbyConnection.hpp>
#include <LetoAPI_V1/Lobby/LobbyInfo.hpp>

#include <LetoAPI_V1/Web/WebConnection.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.hpp>

#include <LetoAPI_V1_System/Web/WebPacket.hpp>
#include <LetoAPI_V1_System/Lobby/LobbyGlobals.hpp>

#include <Data/StaticList.hpp>
#include <Data/RingFIFO_Static.hpp>

#include <Time/Timer.hpp>

/**
 * @brief Class handling network interaction
 * 
 * @warning Available only at OS level. Creation within an application results in UB (Undefined Behavior).
 */
class LIBRARIES_EXPORT LobbyManager_V1
{
protected:
    // ======================================== Singleton ========================================

	LobbyManager_V1();
	~LobbyManager_V1();
	LobbyManager_V1(const LobbyManager_V1&) = delete;
	void operator=(const LobbyManager_V1&) = delete;

    // ===========================================================================================

    Timer find_timer;
    WebConnection_V1 find_connection;   ///< Connection to WC_PORT_FND_LOBBY
    WebConnection_V1 main_conection;    ///< Connection to WC_PORT_CON_LOBBY

    bool ready{};

    /**
     * @brief Check if user is member of current lobby
     * 
     * @param[in] id User identifier
     * @param[out] pos User position in the list
     */
    bool IsMember(uint32_t id, uint32_t* pos = nullptr) const;

    /**
     * @brief Send common information about active lobby
     */
    void SendCommonInfo() const;

    /**
     * @brief Send information about connected members
     * 
     * @param[in] part Index of user triad to send (0 or 1)
     * 
     * @warning This function is executed by the server.
     */
    void SendMembersInfo(uint32_t part) const;

public:
    static inline LobbyManager_V1& Instance()
    {
        static LobbyManager_V1 instance;
        return instance;
    }

	template <typename Tag = void>
	static inline LobbyManager_V1& TagInstance()
    {
        static LobbyManager_V1 instance;
        return instance;
    }

    /** ========================= Functions available in LetoAPI_V1 =========================  */

    /**
	 * @brief Get list of available game lobbies nearby
	 * 
	 * @param[out] info Array to receive information
	 * @param[in] available Size of the destination array
	 *
	 * @return Number of game lobbies
	 */
    uint32_t GetLobbiesNear(LobbyInfo_V1* info, uint32_t available);

    /**
     * @brief Create lobby
     * 
     * @param[out] lobby Opened lobby connection instance
     * @param[in] app_id Application identifier
     * @param[in] max_count Maximum number of connections
     */
    bool CreateLobby(LobbyConnection_V1* lobby, uint16_t app_id, uint8_t max_count, LobbyConnection_V1_Callback callback);

    /**
     * @brief Join lobby
     * 
     * @param[in] info Lobby information
     * @param[in] callback Callback function to receive data from the lobby
     */
    void JoinLobby(LobbyInfo_V1 info, LobbyConnection_V1_Callback callback);

    /**
     * @brief Check for active lobby connection
     */
    bool GetActiveLobby(LobbyConnection_V1* lobby);
    
    /**
     * @brief Exit current lobby
     * 
     * Can be invoked by both lobby creator and member.
     */
    void QuitLobby();

    /**
     * @brief Disconnect member from lobby
     */
    void DisconnectMember(uint32_t ID);

    /**
	 * @brief Send unreliable data to active lobby
	 *
	 * @param[in] data Data to send
	 * @param[in] size Size of the data to send
	 */
	void SendData(const void* data, uint32_t size);

    /**
     * @brief Set readiness for connection.
     * 
     * @param[in] ready True if ready for connection, false otherwise.
     */
    void SetReady(bool ready);

    /** 
     * @brief Get readiness for connection.
     * 
     * @return True if ready for connection, false otherwise.
     */
    bool GetReady();

    /** ==================================================================================  */

    /**
     * @brief Check for pending lobby connection request
     */
    bool GetJoiningLobby(LobbyConnection_V1* lobby);

    /**
     * @brief Global state reset
     */
    void GlobalReset();

	/**
	 * @brief Execute background tasks
	 */
    void Loop();
};

#endif
