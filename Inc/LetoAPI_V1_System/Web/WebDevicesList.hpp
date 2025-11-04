/**
 * @file WebDevicesList.hpp
 * @date Jun 08, 2026
 * @author Rakhimov T.
 */

#ifndef INC_LETO_API_V1_WEB_WEB_DEVICES_LIST_HPP_
#define INC_LETO_API_V1_WEB_WEB_DEVICES_LIST_HPP_

#include "LibrariesExport.h"

#include <Data/StaticList.hpp>
#include <LetoAPI_V1/Web/WebDeviceInfo.hpp>

const uint32_t MaxWebDevices = 8;

struct WebDeviceInfo_ListItem
{
    WebDeviceInfo_V1 device;
    
    /// Последняя активность устройства
    uint32_t last_activity;

    /// Последняя активность устройства в приложении
    uint32_t last_app_activity;
};

// Список с активными на данный момент устройствами
extern LIBRARIES_EXPORT StaticList<WebDeviceInfo_ListItem, MaxWebDevices> WebDevicesList;

#endif /* INC_LETO_API_V1_WEB_WEB_DEVICES_LIST_HPP_ */