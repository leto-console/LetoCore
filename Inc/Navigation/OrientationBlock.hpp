/*
 * OrientationModule.hpp
 *
 *  Created on: Jan 2, 2025
 *      Author: Timur
 */

#ifndef INC_NAVIGATION_ORIENTATIONBLOCK_HPP_
#define INC_NAVIGATION_ORIENTATIONBLOCK_HPP_

#include "Quaternion/Quaternion.hpp"
#include "NavigationTypes.hpp"

using namespace NavigationTypes;

class OrientationBlock
{
	bool initialized	= false;
	bool aligned 		= false;
	Quaternion Q_derivative;
public:
	OrientationBlock();

	void Init();
	void InitByGravity();
	void InitByAngles(const Angles& /*angles*/);
	void InitByQuaternion(const Quaternion& /*q*/);

	void Reset();

	void Tick(uint32_t /*dt_ms*/);

	bool IsInitialized() 	const;
	bool IsAligned() 		const;

	Angles		GetAnglesByGravity()	const;
protected:
	Quaternion 	KinematicEquation(const AbsoluteAngularVelocityVector&);
	Quaternion 	KinematicEquation(const AbsoluteAngularVelocityVector&, const AbsoluteAngularVelocityVector_GCS&);
	void		IntegrateQuaterion(const Quaternion& new_Q_derivative, uint32_t dt_ms);
};

#endif /* INC_NAVIGATION_ORIENTATIONBLOCK_HPP_ */
