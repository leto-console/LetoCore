/*
 * Account.hpp
 *
 *  Created on: Jan 04, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_ACCOUNT_HPP_
#define INC_AUTH_ACCOUNT_HPP_

#include <stdint.h>
#include <Data/StaticText.hpp>

// Объект аккаунта Пользователя
struct Account
{
	uint32_t ID;				// Уникальный идентификатор начинающийся с 1 и до 65535 (или какое-то другое граничное число)
	uint32_t Avatar;			// Идентификатор аватарки в системе
	StaticText<10> Name;		// 10 символов для имени
};

#endif
