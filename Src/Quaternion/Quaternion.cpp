/*
 * Quaternion.cpp
 *
 *  Created on: Dec 25, 2024
 *      Author: Timur
 */

#include "Quaternion/Quaternion.hpp"

#include <cmath>

Quaternion::Quaternion():
	Quaternion{ 0, 0, 0, 0 }
{
}

Quaternion::Quaternion(float q0, float q1, float q2, float q3) :
	q0 { q0 }, q1 { q1 }, q2 { q2 }, q3 { q3 }
{
}

Quaternion::Quaternion(const Quaternion& q) :
	q0 { q.q0 }, q1 { q.q1 }, q2 { q.q2 }, q3 { q.q3 }
{
}

Quaternion Quaternion::scalar(float q0)
{
	return Quaternion(q0, 0, 0, 0);
}

Quaternion Quaternion::operator + (const Quaternion& other) const
{
	Quaternion copy{ *this };
	return copy += other;
}

Quaternion Quaternion::operator - (const Quaternion& other) const
{
	Quaternion copy{ *this };
	return copy -= other;
}

Quaternion Quaternion::operator - () const
{
	Quaternion copy { *this };
	copy.q0 *= -1.;
	copy.q1 *= -1.;
	copy.q2 *= -1.;
	copy.q3 *= -1.;
	return copy;
}

Quaternion Quaternion::operator * (const Quaternion& other) const
{
	Quaternion copy{ *this };
	return copy *= other;
}

Quaternion Quaternion::operator * (const float& other) const
{
	Quaternion copy{ *this };
	return copy *= other;
}

Quaternion& Quaternion::operator += (const Quaternion& other)
{
	this->q0 += other.q0;
	this->q1 += other.q1;
	this->q2 += other.q2;
	this->q3 += other.q3;
	return *this;
}

Quaternion& Quaternion::operator -= (const Quaternion& other)
{
	this->q0 += other.q0;
	this->q1 += other.q1;
	this->q2 += other.q2;
	this->q3 += other.q3;
	return *this;
}

Quaternion& Quaternion::operator *= (const Quaternion& other)
{
	float new_q0 = q0 * other.q0 - q1 * other.q1 - q2 * other.q2 - q3 * other.q3;
	float new_q1 = q0 * other.q1 + q1 * other.q0 + q2 * other.q3 - q3 * other.q2;
	float new_q2 = q0 * other.q2 - q1 * other.q3 + q2 * other.q0 + q3 * other.q1;;
	float new_q3 = q0 * other.q3 + q1 * other.q2 - q2 * other.q1 + q3 * other.q0;
	this->q0 = new_q0;
	this->q1 = new_q1;
	this->q2 = new_q2;
	this->q3 = new_q3;
	return *this;
}

Quaternion& Quaternion::operator *= (const float& other)
{
	this->q0 *= other;
	this->q1 *= other;
	this->q2 *= other;
	this->q3 *= other;
	return *this;
}

Quaternion Quaternion::conjigugate() const
{
	Quaternion copy{ *this };
	copy.q1 *= -1.;
	copy.q2 *= -1.;
	copy.q3 *= -1.;
	return copy;
}

Quaternion Quaternion::multiplicativeInverse() const
{
	Quaternion conjiguatedQ = this->conjigugate();
	double normQ = this->norm();

	Quaternion result = {
		(float)(conjiguatedQ.q0 / normQ),
		(float)(conjiguatedQ.q1 / normQ),
		(float)(conjiguatedQ.q2 / normQ),
		(float)(conjiguatedQ.q3 / normQ)
	};
	return result;
}

float Quaternion::module() const
{
#ifdef USE_HAL_DRIVER
	return std::sqrt(this->norm());
#else
	return sqrtf(this->norm());
#endif
}

float Quaternion::norm() const
{
	return (q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
}


