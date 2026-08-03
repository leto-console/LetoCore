/**
 * @file WebDevicesList.hpp
 * @date Jun 08, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_DEVICES_LIST_HPP_
#define INC_LETO_API_V1_WEB_WEB_DEVICES_LIST_HPP_

#include "LibrariesExport.h"

#include <Data/StaticList.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.h>

const uint32_t MaxWebDevices = 8;

struct WebDeviceInfo_ListItem
{
    WebDeviceInfo_V1 device;
    
    /// Last device activity timestamp
    uint32_t last_activity;

    /// Last device activity in application
    uint32_t last_app_activity;
};

// List of currently active devices
extern LETO_CORE_EXPORT StaticList<WebDeviceInfo_ListItem, MaxWebDevices> WebDevicesList;

#endif /* INC_LETO_API_V1_WEB_WEB_DEVICES_LIST_HPP_ */