/*
 * AccountSettingsCells.hpp
 *
 *  Created on: Mar 01, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_ACCOUNT_SETTINGS_CELLS_HPP_
#define INC_AUTH_ACCOUNT_SETTINGS_CELLS_HPP_

#include <Data/DataCell.hpp>
#include <Data/StaticText.hpp>
#include <cinttypes>

struct AccountSettingsCells
{
	DataCell<uint32_t>			avatar;
	DataCell<StaticText<10>>	name;
};

#endif
