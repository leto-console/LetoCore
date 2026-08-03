/*
 * NavigationTypes.hpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#ifndef INC_NAVIGATION_NAVIGATIONTYPES_HPP_
#define INC_NAVIGATION_NAVIGATIONTYPES_HPP_

#include <stdint.h>

namespace NavigationTypes
{

	enum Projection
	{
		X, Y, Z
	};

	struct Angles
	{
		float yaw;		// Рысканье в радианах
		float pitch;	// Тангаж в радианах
		float roll;	// Крен в радианах
	};

	struct XYZ_Projections
	{
		float x;	// Проекция на ось X
		float y;	// Проекция на ось Y
		float z;	// Проекция на ось Z
		XYZ_Projections() = default;
		XYZ_Projections(float x, float y, float z) :
			x{ x }, y{ y }, z{ z }
		{
		}

		float GetProjection(Projection projection) const
		{
			switch(projection)
			{
			case Projection::X:
				return x;
			case Projection::Y:
				return y;
			case Projection::Z:
				return z;
			default:
				return 0.0f;
			}
		}
	};

	struct FlatCoordinates : XYZ_Projections
	{
	};

	// Вектор кажущегося ускорения в базисе связной СК
	struct ApparentAccelerationVector : XYZ_Projections
	{
	};

	// Вектор кажущегося ускорения в базисе географической СК
	struct ApparentAccelerationVector_GCS : XYZ_Projections
	{
		ApparentAccelerationVector_GCS(float x, float y, float z) :
			XYZ_Projections(x, y, z)
		{
		}
	};

	// Вектор абсолютной угловой скорости в базисе связной СК
	struct AbsoluteAngularVelocityVector : XYZ_Projections
	{
	};

	// Вектор абсолютной угловой скорости географической СК
	struct AbsoluteAngularVelocityVector_GCS : XYZ_Projections
	{
	};

	// Вектор скорости в базисе связной СК
	struct VelocityVector : XYZ_Projections
	{
		VelocityVector(float vx, float vy, float vz) :
			XYZ_Projections(vx, vy, vz)
		{
		}
	};

	// Вектор скорости в базисе географической СК
	struct VelocityVector_GCS : XYZ_Projections
	{
		VelocityVector_GCS(float vx, float vy, float vz) :
			XYZ_Projections(vx, vy, vz)
		{
		}
	};

}

#endif /* INC_NAVIGATION_NAVIGATIONTYPES_HPP_ */
