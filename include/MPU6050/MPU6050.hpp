/*
 * MPU6050.hpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#ifndef INC_MPU6050_MPU6050_HPP_
#define INC_MPU6050_MPU6050_HPP_

#include "LibrariesExport.h"

#include <cstdio>
#include <stdint.h>

#ifdef USE_HAL_DRIVER

#include <HAL_include/HAL.hpp>

#endif

class LETO_CORE_EXPORT MPU6050
{
public:
	enum AFS_SEL
	{
		PLUS_MUNIS_2G,
		PLUS_MUNIS_4G,
		PLUS_MUNIS_8G,
		PLUS_MUNIS_16G,
	};
	enum FS_SEL
	{
		PLUS_MUNIS_250DPS,
		PLUS_MUNIS_500DPS,
		PLUS_MUNIS_1000DPS,
		PLUS_MUNIS_2000DPS,
	};

protected:
#ifdef USE_HAL_DRIVER
	I2C_HandleTypeDef* hi2c;
#endif

	bool system_ok{ false };

	int16_t nx_raw{};
	int16_t ny_raw{};
	int16_t nz_raw{};

	int16_t wx_raw{};
	int16_t wy_raw{};
	int16_t wz_raw{};

	int16_t temp_raw{};

	AFS_SEL afs_sel{ PLUS_MUNIS_4G };
	FS_SEL 	fs_sel{ PLUS_MUNIS_500DPS };

public:

#ifdef USE_HAL_DRIVER
	MPU6050(I2C_HandleTypeDef* hi2c, AFS_SEL afs_sel = PLUS_MUNIS_4G, FS_SEL fs_sel = PLUS_MUNIS_500DPS);
#else
	MPU6050(AFS_SEL afs_sel = PLUS_MUNIS_4G, FS_SEL fs_sel = PLUS_MUNIS_500DPS);
#endif

	bool Init();
	void ReadAll();

	void SetAFS_SEL(AFS_SEL afs_sel) { this->afs_sel = afs_sel; }
	void SetFS_SEL(FS_SEL fs_sel) { this->fs_sel = fs_sel; }

	bool IsSystemOK() const { return system_ok; }

protected:
	void ToLinkedCS(float& x, float& y, float& z) const;

	void InvalidateAll();

	void CheckSystem();
};



#endif /* INC_MPU6050_MPU6050_HPP_ */
