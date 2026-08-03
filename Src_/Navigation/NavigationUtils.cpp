/*
 * Navigation.cpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#include <Navigation/NavigationUtils.hpp>
#include <cmath>

Angles NavigationUtils::AnglesFromGravityProjections(const ApparentAccelerationVector& n_vector)
{
	Angles angles = {
		0.0f,
		atan2f(n_vector.x, n_vector.y),
		atan2f(-n_vector.z, n_vector.y)
	};

	return angles;
}

Angles NavigationUtils::AnglesFromQuaternion(const QuaternionData& data)
{
	Quaternion q{}; data.Get(q);
	return AnglesFromQuaternion(q);
}

Angles NavigationUtils::AnglesFromQuaternion(const Quaternion& q)
{
	Angles angles = {
		atan2f(2 * q.q0 * q.q2 - 2 * q.q1 * q.q3, 2 * q.q1 * q.q1 + 2 * q.q0 * q.q0 - 1),
		asinf(2 * q.q1 * q.q2 + 2 * q.q0 * q.q3),
		atan2f(2 * q.q0 * q.q1 - 2 * q.q2 * q.q3, 2 * q.q0 * q.q0 + 2 * q.q2 * q.q2 - 1)
	};
	return angles;
}

ApparentAccelerationVector_GCS NavigationUtils::getApparentAccelerationVector_GCS(const ApparentAccelerationVector& n, const Quaternion& q)
{
	Quaternion N 	= QuaternionFromProjections(n);
	Quaternion Ng 	= q * N * q.conjigugate();
	ApparentAccelerationVector_GCS ng_vector { Ng.q1, Ng.q2, Ng.q3 };
	return ng_vector;
}

Quaternion NavigationUtils::QuaternionFromAngles(const AnglesData& data)
{
	Angles a{}; data.Get(a);
	return QuaternionFromAngles(a);
}

Quaternion NavigationUtils::QuaternionFromAngles(const Angles& angles)
{
	float sin_yaw 		= sinf(angles.yaw	/ 2.f);
	float sin_pitch 	= sinf(angles.pitch	/ 2.f);
	float sin_roll 		= sinf(angles.roll	/ 2.f);

	float cos_yaw 		= cosf(angles.yaw	/ 2.f);
	float cos_pitch 	= cosf(angles.pitch	/ 2.f);
	float cos_roll 		= cosf(angles.roll	/ 2.f);

	Quaternion quaternion = {
			(cos_yaw * cos_pitch * cos_roll - sin_yaw * sin_pitch * sin_roll),
			(cos_yaw * cos_pitch * sin_roll - sin_yaw * sin_pitch * cos_roll),
			(sin_yaw * cos_pitch * cos_roll - cos_yaw * sin_pitch * sin_roll),
			(cos_yaw * sin_pitch * cos_roll - sin_yaw * cos_pitch * sin_roll)
	};

	return quaternion;
}

Quaternion NavigationUtils::QuaternionFromProjections(const XYZ_Projections& p)
{
	return Quaternion{ 0.0f, p.x, p.y, p.z };
}
