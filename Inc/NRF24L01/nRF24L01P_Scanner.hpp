/*
 * nRF24L01P_Scanner.hpp
 *
 *  Created on: Jan 17, 2025
 *      Author: Timur
 */

#ifndef INC_NRF24L01_SCANNER_HPP_
#define INC_NRF24L01_SCANNER_HPP_

#include "LibrariesExport.h"

#include <Data/DataCell.hpp>
#include <Drawable/IDrawable.hpp>
#include <Time/TimeUtils.hpp>
#include <Utils/random.hpp>

#ifdef USE_HAL_DRIVER
#include <NRF24L01/nrf24l01.h>
#endif

#include <stdint.h>

#include <DrawFunctions/DrawLine.hpp>

class LETO_CORE_EXPORT nRF24L01P_Scanner : public IDrawable
{
protected:
	int				ChannelScan = 0;
	uint8_t			Loading = 0;           
	uint8_t			ChannelPowerNow[128]{};
	const uint8_t	Iterations = 32;

public:
	nRF24L01P_Scanner()
	{
		SetActive(false);
	}
	
	void Reset()
	{
		ChannelScan = 0;
		memset(ChannelPowerNow, 0, sizeof(ChannelPowerNow));
	}

	void Draw(IScreen& screen, Point2_i offset = {}) override
	{
		using namespace DrawFunctions;

		// Режим сканирования
		screen.ClearScreen();
		DrawLine(screen, { ChannelScan, 0 }, { ChannelScan, 4 }, WhiteColor);

		for (int i = 0; i < 128; i++)
			DrawLine(screen, { i, 64 - ChannelPowerNow[i] * 2 }, { i, 64 }, WhiteColor);
	}

	void Loop() override
	{
		if (!IsActive())
			return;

#ifdef USE_HAL_DRIVER
		setAutoAck(false);
		setChannel(ChannelScan);                                       
		startListening();                                              
		delay_us(50);
		stopListening();                                               
		if (read_register(RPD) & 1) { ChannelPowerNow[ChannelScan]++; }
#else
		TimeUtils::SleepMks(50);
		if (Random(0, 4) == 0) { ChannelPowerNow[ChannelScan]++; }  
#endif

		if (Loading < Iterations) {                                    
			Loading++;                                                 
		}
		else
		{
			ChannelScan = (ChannelScan + 1) % 128;                            
			Loading = 0;
			ChannelPowerNow[ChannelScan] = 0;                                 
		}
	}
};

#endif
