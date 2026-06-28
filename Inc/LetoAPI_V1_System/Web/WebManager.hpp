/**
 * @file WebManager.hpp
 * @date Jun 04, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_MANAGER_HPP_
#define INC_LETO_API_V1_WEB_WEB_MANAGER_HPP_

#include "LibrariesExport.h"

#include <cstdint>

#include <LetoAPI_V1/Web/WebConnection.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.hpp>

#include <LetoAPI_V1_System/Web/WebPacket.hpp>

#include <Data/StaticList.hpp>
#include <Data/RingFIFO_Static.hpp>

#include <Time/Timer.hpp>

const uint32_t POOL_SIZE = 8;

struct WebPacket_BufferItem
{
	WebPacket_V1 packet;			///< Packet
	uint8_t channel;				///< Channel
};

struct WebPacket_SyncItem
{
	WebPacket_BufferItem buffer_item;	///< Packet and channel
	uint32_t last_send_ms;				///< Last send timestamp
};

/**
 * @brief Helper object for managing open connections
 */
struct WebConnection_PoolItem
{
	WebConnection_V1 connection;    ///< Connection data
    uint32_t last_active_ms;        ///< Last connection activity timestamp
};

/**
 * @brief Class handling network interaction
 * 
 * @warning Available only at OS level; creating in application causes UB
 */
class LIBRARIES_EXPORT WebManager_V1
{
private:
    // ======================================== Singleton ========================================

	WebManager_V1() = default;
	~WebManager_V1() = default;
	WebManager_V1(const WebManager_V1&) = delete;
	void operator=(const WebManager_V1&) = delete;

    // ===========================================================================================

	RingFIFO_Static<WebPacket_BufferItem, 32> buffer;			///< Outgoing messages buffer (read by send task)
	uint32_t buffer_pps{};	///< Packets/sec
	uint32_t buffer_cnt{};	///< Counter
	Timer buffer_sec_timer{ 1000 };

    StaticList<WebConnection_PoolItem, POOL_SIZE> pool;

	StaticList<WebPacket_SyncItem, 8> sync_out;		///< Output messages buffer with delivery guarantee
	StaticList<WebPacket_SyncItem, 8> sync_in;		///< Input messages buffer with delivery guarantee

	WebDeviceInfo_V1 self_info;

    bool FindConnection(WebConnection_PoolItem** connection, uint8_t channel, uint8_t port, uint32_t id);

public:
    static inline WebManager_V1& Instance()
    {
        static WebManager_V1 instance;
        return instance;
    }

	template <typename Tag = void>
	static inline WebManager_V1& TagInstance()
    {
        static WebManager_V1 instance;
        return instance;
    }

	uint32_t BufferCount() const { return buffer.Count(); }

	/// Web packets per second
	uint32_t BufferPPS() const { return buffer_pps; }

	/** ========================= Functions available in LetoAPI_V1 =========================  */

    /**
	 * @brief Create connection
	 * 
     * @param[out] connection Open connection
	 * @param[in] channel Connection channel (COM, nRF, other)
	 * @param[in] port Connection port
	 * @param[in] id Device identifier for connection (0 for broadcast)
	 * @param[in] callback Callback function on data receive (can be NULL)
     * 
     * @return `true` if connection successfully created
	 */
    bool CreateConnection(WebConnection_V1* connection, uint8_t channel, uint8_t port, uint32_t id, WebConnection_V1_Callback callback);

    /**
	 * @brief Close connection
	 *
	 * @param[in] connection Open connection
	 */
	void CloseConnection (WebConnection_V1 connection);

    /**
	 * @brief Send data without delivery guarantee
	 *
	 * @param[in] connection Open connection
	 * @param[in] data Data to send
	 * @param[in] size Data size to send
	 */
	void SendData (WebConnection_V1 connection, const void* data, uint32_t size);

    /**
	 * @brief Get list of nearby devices
	 * 
	 * @param[out] info Array for receiving information
	 * @param[in] available Array size for receiving information
	 *
	 * @return Number of devices
	 */
	uint32_t GetDevicesNear (WebDeviceInfo_V1* info, uint32_t available) const;

	/**
	 * @brief Find device by identifier
	 * 
	 * @param[in] id Device identifier
	 * @param[out] info Device information
	 * 
	 * @return `true` if device successfully found
	 */
	bool FindDeviceNear(uint32_t id, WebDeviceInfo_V1* info) const;

	/** ==================================================================================  */

	/**
	 * @brief Set self_info WebDeviceInfo
	 */
	void SetSelfInfo(WebDeviceInfo_V1 info);

	/**
	 * @brief Get self_info WebDeviceInfo
	 */
	WebDeviceInfo_V1 GetSelfData() const;

    /**
     * @brief Get list of open connections
     * 
     * @param[out] info Array for receiving information
	 * @param[in] available Array size for receiving information
	 *
	 * @return Number of connections
     */
    uint32_t GetConnections(WebConnection_V1* info, uint32_t available);

	/**
	 * @brief Send data with delivery guarantee
	 * 
	 * @param[in] connection Open connection
	 * @param[in] data Data to send
	 * @param[in] size Data size to send
	 */
	void SendSynchronizeData(WebConnection_V1 connection, const void* data, uint32_t size);

	/**
	 * @brief Receive data
	 * 
	 * @param[in] channel Connection channel (COM, nRF, other)
	 * @param[in] port Connection port
	 * @param[in] id Device identifier for connection (0 for broadcast)
	 * @param[in] data Data to receive
	 * @param[in] size Data size to receive
	 */
	void ReceiveData(uint8_t channel, const WebPacket_V1& packet);

	/**
	 * @brief Check for packet to send
	 * 
	 * @param[out] item Structure with information about packet to send
	 * 
	 * @return Packet availability
	 */
	bool CheckBuffer(WebPacket_BufferItem& info);

	/**
	 * @brief Switch to next packet to send
	 */
	void NextBuffer();

	/**
	 * @brief Execute background actions
	 */
	void Loop();
};


#endif