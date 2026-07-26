/**
 * @file DateTimeServer.hpp
 * @date Jul 26, 2026
 * @author Rakhimov T.
 */

#ifndef INC_TIME_DATE_TIME_SERVER_HPP_
#define INC_TIME_DATE_TIME_SERVER_HPP_

#include <cstdint>

#include <TaskHandler/PriorityTask.hpp>

class LIBRARIES_EXPORT DateTimeServer : public PriorityTask
{
public:
	DateTimeServer(const StaticText32& name, uint32_t period_ms, uint8_t priority)
        : PriorityTask{ name, period_ms, priority }
    { }

	bool Do() override;
};

#endif
