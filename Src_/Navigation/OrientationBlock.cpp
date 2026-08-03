/*
 * OrientationBlock.cpp
 *
 *  Created on: Jan 2, 2025
 *      Author: Timur
 */

#include "Navigation/OrientationBlock.hpp"
#include "Navigation/NavigationUtils.hpp"

#include "MPU6050/MPU6050Data.hpp"
#include "Navigation/NavData.hpp"

OrientationBlock::OrientationBlock()
{
}

void OrientationBlock::Init()
{
	// Значение по умолчанию 0.4f
	Complement_Koeff.Set(0.4f);
	initialized = true;
}

void OrientationBlock::InitByGravity()
{
	InitByAngles(GetAnglesByGravity());
}

void OrientationBlock::InitByAngles(const Angles& angles)
{
	Orientation_Quaternion.Set(NavigationUtils::QuaternionFromAngles(angles));
	Init();
}

void OrientationBlock::InitByQuaternion(const Quaternion& q)
{
	Orientation_Quaternion.Set(q);
	Init();
}

void OrientationBlock::Reset()
{
	Angles empty{};
	
	Complement_Angles.Set(empty);
	Gravity_Angles.Set(empty);
	InitByGravity();
}

void OrientationBlock::Tick(uint32_t dt_ms)
{
	AbsoluteAngularVelocityVector w{};
	Linked_Wx.Get(w.x);
	Linked_Wy.Get(w.y);
	Linked_Wz.Get(w.z);

	Quaternion new_Q_derivative = KinematicEquation(w);
	IntegrateQuaterion(new_Q_derivative, dt_ms);

	Angles byGravity = GetAnglesByGravity();
	Gravity_Angles.Set(byGravity);

	// Комплементарный фильтр
	// a(t) = (1 - K) * (a(t - 1) + gx * dt) + K * acc[3]

	float K;
	Complement_Koeff.Get(K);

	Angles Complement;
	Complement_Angles.Get(Complement);

	Complement.pitch	= (1 - K) * (Complement.pitch	+ w.y * dt_ms / 1000.f) + K * byGravity.pitch;
	Complement.roll		= (1 - K) * (Complement.roll	+ w.z * dt_ms / 1000.f) + K * byGravity.roll;
	Complement_Angles.Set(Complement);
}

bool OrientationBlock::IsInitialized() const
{
	return initialized;
}

bool OrientationBlock::IsAligned() const
{
	return aligned;
}

Angles OrientationBlock::GetAnglesByGravity() const
{
	ApparentAccelerationVector vector{};
	Linked_Nx.Get(vector.x);
	Linked_Ny.Get(vector.y);
	Linked_Nz.Get(vector.z);

	return NavigationUtils::AnglesFromGravityProjections(vector);
}

Quaternion OrientationBlock::KinematicEquation(const AbsoluteAngularVelocityVector& w)
{
	return KinematicEquation(w, {});
}

Quaternion OrientationBlock::KinematicEquation(const AbsoluteAngularVelocityVector& w, const AbsoluteAngularVelocityVector_GCS& w_gcs)
{
	Quaternion Q_derivative_local;
	Quaternion W 		= NavigationUtils::QuaternionFromProjections(w);
	Quaternion W_GCS 	= NavigationUtils::QuaternionFromProjections(w_gcs);
	Quaternion Q; Orientation_Quaternion.Get(Q);
	Q_derivative_local 	= Q * W - W_GCS * Q + Q * (1.f - Q.norm());
	return Q_derivative_local * 0.5f;
}

void OrientationBlock::IntegrateQuaterion(const Quaternion& new_Q_derivative, uint32_t dt_ms)
{
	float dq0 = (new_Q_derivative.q0 + Q_derivative.q0) / 2.f;
	float dq1 = (new_Q_derivative.q1 + Q_derivative.q1) / 2.f;
	float dq2 = (new_Q_derivative.q2 + Q_derivative.q2) / 2.f;
	float dq3 = (new_Q_derivative.q3 + Q_derivative.q3) / 2.f;

	Quaternion Q; Orientation_Quaternion.Get(Q);
	Q.q0 += (dq0 * (float) dt_ms / 1000.f);
	Q.q1 += (dq1 * (float) dt_ms / 1000.f);
	Q.q2 += (dq2 * (float) dt_ms / 1000.f);
	Q.q3 += (dq3 * (float) dt_ms / 1000.f);
	Orientation_Quaternion.Set(Q);

	Q_derivative = new_Q_derivative;
}
