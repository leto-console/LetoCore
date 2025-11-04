/*
 * WebDevicesTask.hpp
 *
 *  Created on: Feb 26, 2026
 *      Author: Timur
 */

#ifndef INC_WEB_WEB_DEVICES_TASK_HPP_
#define INC_WEB_WEB_DEVICES_TASK_HPP_

#include "LibrariesExport.h"

#include <TaskHandler/PriorityTask.hpp>
#include <Time/Timer.hpp>
#include <Auth/Account.hpp>
#include <Data/StaticText.hpp>

#include <LetoAPI_V1/Web/WebConnection.hpp>

class LIBRARIES_EXPORT WebDevicesTask : public PriorityTask
{
protected:
	WebConnection_V1 connection;	
	Account account;
	Timer refresh_timer;

	bool inited;

	void RefreshDevicesList();

	bool UpdateInfo();
	void SendAccountMsg();
	void SendAppMsg();

	bool Do() override;

public:
	WebDevicesTask(const StaticText32& name, uint32_t period_ms, uint8_t priority);
	virtual ~WebDevicesTask();
};

#endif