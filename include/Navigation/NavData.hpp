/*
 * NavData.hpp
 *
 *  Created on: Oct 13, 2025
 *      Author: Timur
 */

#ifndef INC_NAVIGATION_NAVDATA_HPP_
#define INC_NAVIGATION_NAVDATA_HPP_

#include <Data/DataCell.hpp>
#include "Quaternion/Quaternion.hpp"
#include "NavigationTypes.hpp"

using AnglesData		= DataCell<NavigationTypes::Angles>;
using QuaternionData	= DataCell<Quaternion>;

// Углы вычисленные гравитационным способом
extern AnglesData Gravity_Angles;

// Коэффициент для вычисления углов комплементарным способом (акселерометр + гироскоп)
extern DataCell<float> Complement_Koeff;

// Углы вычисленные комплементарным способом (акселерометр + гироскоп)
extern AnglesData Complement_Angles;

 // Кватернион ориентации
extern QuaternionData Orientation_Quaternion;

#endif /* INC_NAVIGATION_NAVDATA_HPP_ */
