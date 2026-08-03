/*
 * MPU6050.cpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#include "MPU6050/MPU6050.hpp"

#include "MPU6050/MPU6050Data.hpp"

#ifndef USE_HAL_DRIVER
#include "Utils/FileConfig.hpp"
#endif

#define MPU6050_TIMEOUT 	1000
#define MPU6050_Address 	0xD0

#define SMPLRT_DIV_REG 		0x19
#define ACCEL_CONFIG_REG 	0x1C
#define GYRO_CONFIG_REG 	0x1B
#define INT_ENABLE_REG 		0x38
#define ACCEL_XOUT_H		0x3B
#define PWR_MGMT_1_REG 		0x6B
#define WHO_AM_I_REG  		0x75

static float MPU6050_AFS_SEL_LSB[4] 	= {16384.f, 8192.f, 4096.f, 2048.f};
static float MPU6050_FS_SEL_LSB[4] 	= {131.f, 65.5f, 32.8f, 1.f};

constexpr size_t MPU6050_Source = 0;

#ifdef USE_HAL_DRIVER
MPU6050::MPU6050(I2C_HandleTypeDef* hi2c, AFS_SEL afs_sel, FS_SEL fs_sel):
		hi2c{ hi2c }, afs_sel{ afs_sel }, fs_sel{ fs_sel }
{
}
#else
MPU6050::MPU6050(AFS_SEL afs_sel, FS_SEL fs_sel) :
	afs_sel{ afs_sel }, fs_sel{ fs_sel }
{
}
#endif

bool MPU6050::Init()
{
    uint8_t check = 0;
	uint8_t data = 0;

    // check device ID WHO_AM_I
#ifdef USE_HAL_DRIVER
	HAL_I2C_Mem_Read(hi2c, MPU6050_Address, WHO_AM_I_REG, 1, &check, 1, MPU6050_TIMEOUT);

	printf("WHO_AM_I_REG = 0x%X: ", check);
	printf(check == 0x68 ? "success" : "fail");
	printf("\r\n");

	system_ok = check == 0x68;
	if (!system_ok)
		return 0;

	data = 0;
	HAL_I2C_Mem_Write(hi2c, MPU6050_Address, PWR_MGMT_1_REG, 1, &data, 1, MPU6050_TIMEOUT);

	data = 0x07;
	HAL_I2C_Mem_Write(hi2c, MPU6050_Address, SMPLRT_DIV_REG, 1, &data, 1, MPU6050_TIMEOUT);

	data = afs_sel << 3;
	HAL_I2C_Mem_Write(hi2c, MPU6050_Address, ACCEL_CONFIG_REG, 1, &data, 1, MPU6050_TIMEOUT);

	data = fs_sel << 3;
	HAL_I2C_Mem_Write(hi2c, MPU6050_Address, GYRO_CONFIG_REG, 1, &data, 1, MPU6050_TIMEOUT);

	data = 0x1;
	HAL_I2C_Mem_Write(hi2c, MPU6050_Address, INT_ENABLE_REG, 1, &data, 1, MPU6050_TIMEOUT);

#else

	FileConfig::InitFloat("mpu6050", "nx");
	FileConfig::InitFloat("mpu6050", "ny");
	FileConfig::InitFloat("mpu6050", "nz");
	FileConfig::InitFloat("mpu6050", "wx");
	FileConfig::InitFloat("mpu6050", "wy");
	FileConfig::InitFloat("mpu6050", "wz");
	FileConfig::InitFloat("mpu6050", "temp");

#endif

	return 1;
}

void MPU6050::ReadAll()
{
#ifdef USE_HAL_DRIVER
	CheckSystem();
	if (!system_ok)
	{
		InvalidateAll();
		return;
	}

	uint8_t data[14];
	HAL_StatusTypeDef res = HAL_I2C_Mem_Read(hi2c, MPU6050_Address, ACCEL_XOUT_H, 1, data, 14, MPU6050_TIMEOUT);

	if (res != HAL_OK)
	{
		system_ok = false;
		InvalidateAll();
		return;
	}

	nx_raw 		= data[0]  << 8 | data[1];
	ny_raw 		= data[2]  << 8 | data[3];
	nz_raw 		= data[4]  << 8 | data[5];
	temp_raw 	= data[6]  << 8 | data[7];
	wx_raw 		= data[8]  << 8 | data[9];
	wy_raw 		= data[10] << 8 | data[11];
	wz_raw 		= data[12] << 8 | data[13];

	// Установка кажущегося ускорения в базисе связной СК
	float nx, ny, nz;
	nx = nx_raw / MPU6050_AFS_SEL_LSB[afs_sel];
	ny = ny_raw / MPU6050_AFS_SEL_LSB[afs_sel];
	nz = nz_raw / MPU6050_AFS_SEL_LSB[afs_sel];
	ToLinkedCS(nx, ny, nz);

	Linked_Nx.SetBySource(nx, MPU6050_Source);
	Linked_Ny.SetBySource(ny, MPU6050_Source);
	Linked_Nz.SetBySource(nz, MPU6050_Source);

	// Установка температуры
	Temperature.SetBySource(temp_raw / 340.0f + 36.53f, MPU6050_Source);

	// Установка абсолютной угловой скорости в базисе связной СК
	float wx, wy, wz;
	wx = wx_raw / MPU6050_FS_SEL_LSB[fs_sel];
	wy = wy_raw / MPU6050_FS_SEL_LSB[fs_sel];
	wz = wz_raw / MPU6050_FS_SEL_LSB[fs_sel];
	ToLinkedCS(wx, wy, wz);

	Linked_Wx.SetBySource(wx, MPU6050_Source);
	Linked_Wy.SetBySource(wy, MPU6050_Source);
	Linked_Wz.SetBySource(wz, MPU6050_Source);

#else
	/* В блоке device_cs_n упакованы 3 float значения n по каждой из проекций (X,Y,Z) */
	bool ok = true;
	ok &= FileConfig::ReadFloat(Linked_Nx, MPU6050_Source, "mpu6050", "nx");
	ok &= FileConfig::ReadFloat(Linked_Ny, MPU6050_Source, "mpu6050", "ny");
	ok &= FileConfig::ReadFloat(Linked_Nz, MPU6050_Source, "mpu6050", "nz");

	/* В блоке device_cs_w упакованы 3 float значения w по каждой из проекций (X,Y,Z) */
	ok &= FileConfig::ReadFloat(Linked_Wx, MPU6050_Source, "mpu6050", "wx");
	ok &= FileConfig::ReadFloat(Linked_Wy, MPU6050_Source, "mpu6050", "wy");
	ok &= FileConfig::ReadFloat(Linked_Wz, MPU6050_Source, "mpu6050", "wz");

	/* В блоке temp упаковано 1 float значения temp */
	ok &= FileConfig::ReadFloat(Temperature, MPU6050_Source, "mpu6050", "temp");

	if (!ok) 
		InvalidateAll();
#endif
}

void MPU6050::ToLinkedCS(float& x, float& y, float& z) const
{
	float temp = z;
	z = -y;
	y = temp;
}

void MPU6050::InvalidateAll()
{
	static FloatSourceData* invalidate[] = {
		&Linked_Nx, &Linked_Ny, &Linked_Nz,
		&Linked_Wx, &Linked_Wy, &Linked_Wz,
		&Temperature
	};
	for (FloatSourceData* data : invalidate)
		data->InvalidateBySource(MPU6050_Source);
}

void MPU6050::CheckSystem()
{
#ifdef USE_HAL_DRIVER
    uint8_t check = 0;
    HAL_StatusTypeDef res = HAL_I2C_Mem_Read(hi2c, MPU6050_Address, WHO_AM_I_REG, 1, &check, 1, MPU6050_TIMEOUT);
    MPU6050_I2CState.SetBySource(res, 0);

	if (res == HAL_OK)
		system_ok = check == 0x68;
	else
		system_ok = false;
#else
	system_ok = true;
#endif
}
