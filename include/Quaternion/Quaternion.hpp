/*
 * Quaternion.hpp
 *
 *  Created on: Dec 25, 2024
 *      Author: Timur
 */

#ifndef INC_QUATERNION_QUATERNION_HPP_
#define INC_QUATERNION_QUATERNION_HPP_

#include "LibrariesExport.h"

#include "stdint.h"

struct LETO_CORE_EXPORT Quaternion
{
	float q0;
	float q1;
	float q2;
	float q3;

	Quaternion();
	Quaternion(float q0, float q1, float q2, float q3);
	Quaternion(const Quaternion& q);
	Quaternion scalar(float q0);

	Quaternion operator + (const Quaternion& other) const;		// = Q1 + Q2
	Quaternion operator - (const Quaternion& other) const;		// = Q1 - Q2
	Quaternion operator - () const;								// = -Q1
	Quaternion operator * (const Quaternion& other) const;		// = Q1 * Q2 - по правилам кватернионов
	Quaternion operator * (const float& other) 		const;		// = Q1 * a

	Quaternion& operator += (const Quaternion& other);			// Q1 += Q2
	Quaternion& operator -= (const Quaternion& other);			// Q1 -= Q2
	Quaternion& operator *= (const Quaternion& other);			// Q1 *= Q2
	Quaternion& operator *= (const float& other);				// Q1 *= a

	Quaternion conjigugate() const;
	Quaternion multiplicativeInverse() const;

	float module() const;
	float norm() const;
};

#endif /* INC_QUATERNION_HPP_ */
