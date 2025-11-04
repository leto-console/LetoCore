/*
 * Navigation.hpp
 *
 *  Created on: Dec 29, 2024
 *      Author: Timur
 */

#ifndef INC_NAVIGATION_NAVIGATIONUTILS_HPP_
#define INC_NAVIGATION_NAVIGATIONUTILS_HPP_

#include "NavigationTypes.hpp"
#include "Quaternion/Quaternion.hpp"
#include "Navigation/NavData.hpp"

using namespace NavigationTypes;

struct NavigationUtils
{
	static Angles AnglesFromGravityProjections(const ApparentAccelerationVector& /*n*/);
	static Angles AnglesFromQuaternion(const QuaternionData& /*q*/);
	static Angles AnglesFromQuaternion(const Quaternion& /*q*/);

	static ApparentAccelerationVector_GCS getApparentAccelerationVector_GCS(const ApparentAccelerationVector& /*n*/, const Quaternion& /*q*/);

	static Quaternion QuaternionFromAngles(const AnglesData& /*data*/);
	static Quaternion QuaternionFromAngles(const Angles& /*angles*/);
	static Quaternion QuaternionFromProjections(const XYZ_Projections& /*n*/);

};

#endif /* INC_NAVIGATION_NAVIGATIONUTILS_HPP_ */
