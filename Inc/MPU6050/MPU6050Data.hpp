/*
 * MPU6050Data.hpp
 *
 *  Created on: Nov 02, 2025
 *      Author: Timur
 */

#ifndef INC_MPU6050_MPU6050DATA_HPP_
#define INC_MPU6050_MPU6050DATA_HPP_

#include "LibrariesExport.h"

#include "Data/SingleData.hpp"
#include "Data/SourceData.hpp"

// Проекция на ось X кажущегося ускорения в базисе связной СК
extern LIBRARIES_EXPORT FloatSourceData Linked_Nx;
// Проекция на ось Y кажущегося ускорения в базисе связной СК
extern LIBRARIES_EXPORT FloatSourceData Linked_Ny;
// Проекция на ось Z кажущегося ускорения в базисе связной СК
extern LIBRARIES_EXPORT FloatSourceData Linked_Nz;

// Проекция на ось X абсолютной угловой скорости связной СК
extern LIBRARIES_EXPORT FloatSourceData Linked_Wx;
// Проекция на ось Y абсолютной угловой скорости связной СК
extern LIBRARIES_EXPORT FloatSourceData Linked_Wy;
// Проекция на ось Z абсолютной угловой скорости связной СК
extern LIBRARIES_EXPORT FloatSourceData Linked_Wz;

// Температура
extern LIBRARIES_EXPORT FloatSourceData Temperature;

// Статус получения данных MPU6050 по I2C
extern LIBRARIES_EXPORT U8SourceData MPU6050_I2CState;

#endif
