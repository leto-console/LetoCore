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

		Reg_00.Set(read_register(0x00));
		Reg_01.Set(read_register(0x01));
		Reg_02.Set(read_register(0x02));
		Reg_03.Set(read_register(0x03));
		Reg_04.Set(read_register(0x04));
		Reg_05.Set(read_register(0x05));
		Reg_06.Set(read_register(0x06));
		Reg_07.Set(read_register(0x07));
		Reg_08.Set(read_register(0x08));
		Reg_09.Set(read_register(0x09));

		Reg_17.Set(read_register(0x17));
		Reg_1C.Set(read_register(0x1C));
		Reg_1D.Set(read_register(0x1D));
#endif
		return true;
	}
};

#endif
