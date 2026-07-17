/**
 * @file ExtDevice.hpp
 * @date Jul 14, 2026
 * @author Rakhimov T.
 */

#ifndef INC_SYSTEM_EXT_DEVICE_HPP_
#define INC_SYSTEM_EXT_DEVICE_HPP_

#include "LibrariesExport.h"

#include <cstdint>

/**
 * @brief External device status
 */
enum class ExtDeviceStatus : uint8_t
{
    UNDEFINED,      ///< Initional status - device is undefined
    READY,          ///< Device is ready for work
    BUSY,           ///< Device connected and busy
    DISCONNECTED,   ///< Device disconnected
    BAD_INIT,       ///< Problems on initialization
    INITIALIZING,   ///< Device is initializing
};

enum class ExtDeviceFeatures : uint32_t
{
    NONE = (0 << 0),
    ON_OFF = (1 << 1)
};

/**
 * @brief Interface of external device
 */
class LIBRARIES_EXPORT ExtDevice
{
public:
    ExtDevice(const char* device_name);
    virtual ~ExtDevice() = default;

    /**
     * @brief Name of external device
     */
    const char* GetName() const;

    /**
     * @brief Init external device asynchronously (awaiting GetStatus call)
     */
    void AsyncInit(uint8_t retries = 3);

    /**
     * @brief Init external device
     */
    void Init(uint8_t retries = 3);

    /**
     * @brief Main loop of ExtDevice processing
     */
    void MainLoop();

    /**
     * @brief Get status of external device
     */
    ExtDeviceStatus GetStatus();

    /**
     * @brief Enable external device
     */
    virtual void Enable() = 0;

    /**
     * @brief Disable external device
     */
    virtual void Disable() = 0;

    /**
     * @brief Get external device features
     */
    virtual ExtDeviceFeatures GetFeatures() = 0;


protected:
    ExtDeviceStatus status{ ExtDeviceStatus::UNDEFINED };

    /**
     * @brief Init external device
     */
    virtual bool DeviceInit() = 0;

    /**
     * @brief Main processing loop for external device
     */
    virtual void DeviceTick() = 0;

private:
    const char* const device_name{};

    uint8_t init_retries{};
    bool init_allow{};
};

#endif
