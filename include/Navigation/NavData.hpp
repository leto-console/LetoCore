/*
 * NavData.hpp
 *
 *  Created on: Oct 13, 2025
 *      Author: Timur
 */

#ifndef INC_NAVIGATION_NAVDATA_HPP_
#define INC_NAVIGATION_NAVDATA_HPP_

#include <Data/SingleData.hpp>
#include "Quaternion/Quaternion.hpp"
#include "NavigationTypes.hpp"

using AnglesData		= SingleData<NavigationTypes::Angles>;
using QuaternionData	= SingleData<Quaternion>;

// Углы вычисленные гравитационным способом
extern AnglesData Gravity_Angles;

// Коэффициент для вычисления углов комплементарным способом (акселерометр + гироскоп)
extern FloatSingleData Complement_Koeff;

// Углы вычисленные комплементарным способом (акселерометр + гироскоп)
extern AnglesData Complement_Angles;

 // Кватернион ориентации
extern QuaternionData Orientation_Quaternion;

#endif /* INC_NAVIGATION_NAVDATA_HPP_ */
