/**
 * @file PPM_Info.hpp
 * @date Jul 29, 2026
 * @author Rakhimov T.
 */

#ifndef INC_TIME_PPM_INFO_HPP_
#define INC_TIME_PPM_INFO_HPP_

#include <cstdint>
#include <cmath>

#ifdef USE_HAL_DRIVER
#include <HAL_include/HAL.hpp>
#endif

#pragma pack(push, 1)

struct PPM_Info
{
    struct PPM_Record
    {
        int32_t ppm;
        int32_t diff_real;
    } 
    records[3]{};

    void AddPPM(int32_t ppm, int32_t diff_real)
    {
        if (ppm < -5000 || ppm > 5000) return;
        records[0].ppm = ppm;
        records[0].diff_real = diff_real;
    }
    
    int32_t GetPPM()
    {
        return records[0].ppm;
    }
};

#pragma pack(pop)

#endif
