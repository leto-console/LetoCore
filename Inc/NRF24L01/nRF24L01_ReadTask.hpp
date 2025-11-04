/*
 * nRF24L01_ReadTask.hpp
 *
 *  Created on: Dec 19, 2025
 *      Author: Timur
 */

#ifndef INC_NRF24L01_READ_TASK_DATA_HPP_
#define INC_NRF24L01_READ_TASK_DATA_HPP_

#include <NRF24L01/NRF24L01_data.hpp>
#include <TaskHandler/PriorityTask.hpp>
#include <Data/StaticText.hpp>

class nRF24L01_ReadTask : public PriorityTask
{
public:
	nRF24L01_ReadTask(const StaticText32& name, uint32_t period_ms, uint8_t priority)
		: PriorityTask{ name, period_ms, priority }
	{
#ifdef USE_HAL_DRIVER
		NRF24L01::PVariant.Set(isPVariant());
#endif
	}
	bool Do() override
	{
		using namespace NRF24L01;
#ifdef USE_HAL_DRIVER
		// REG: 0x00
		Config.Set(read_register(NRF_CONFIG));
		// REG: 0x04
		SetupRetr.Set(read_register(SETUP_RETR));
		// REG: 
		Setup.Set(read_register(RF_SETUP));
		Status.Set(read_register(NRF_STATUS));
		FifoStatus.Set(read_register(FIFO_STATUS));
#endif
		return true;
	}
};

#endif
