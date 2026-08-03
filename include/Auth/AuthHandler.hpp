/*
 * AuthHandler.hpp
 *
 *  Created on: Jan 04, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_AUTH_HANDLER_HPP_
#define INC_AUTH_AUTH_HANDLER_HPP_

#include "LibrariesExport.h"

#include <Auth/Account.hpp>
#include <Storage/Storage.hpp>
#include <Storage/NamedSpace.hpp>

#include <Storage/StoredDataCell.hpp>
#include <Data/StaticText.hpp>
#include <Data/StaticList.hpp>
#include <Data/StaticListView.hpp>

class AuthAccessor;

/*
	Обработчик пользовательской логики
*/
class LETO_CORE_EXPORT AuthHandler
{
protected:
	// Ячейка с индексом текущего пользователя
	StoredDataCell<uint32_t>* cur_ID_cell{ nullptr };

	// Ячейка с ID нового пользователя
	StoredDataCell<uint32_t>* new_ID_cell{ nullptr };

	// Хранилище памяти
	Storage* storage{ nullptr };

	uint16_t accounts_count{};
	uint16_t space_size{};

	mutable StaticList<Account, 16> last_get_accounts{};
	StaticList<StoredDataCell<Account>*, 16> accounts{};
	NamedSpace* current_space{};

	AuthHandler() = default;

	/*
		Получить по ID аккаунта его индекс в массиве
	*/
	bool GetIndexByID(uint32_t ID, uint32_t& idx) const;

public:
	static AuthHandler& Instance()
	{
		static AuthHandler instance;
		return instance;
	}

	/*
		Оценки возможности создания аккаунта
	*/
	bool CanCreateAccount(uint16_t* index = nullptr) const;

	/*
		Создать аккаунт
	*/
	bool CreateAccount(const StaticText32& name, uint32_t avatar, uint32_t& index);

	/*
		Удалить аккаунт
	*/
	bool DeleteAccount(uint32_t ID);

	/*
		Получить список аккаунтов
		TODO: по очередности входа (можно также хранить в памяти)
	*/
	StaticListView<Account> GetAccounts() const;

	/*
		Войти в систему с помощью ID
		TODO: сделать запароленный вход
	*/
	bool Login(uint32_t ID);

	/*
		Выйти из системы
	*/
	void Logout();

	/*
		Проинициализировать адрес в eeprom с авторизационными настройками и держатель памяти.
		Область памяти с авторизационными настройками занимает 16 байт
	*/
	void Init(uint16_t auth_address, Storage& auth_storage, uint16_t auth_accounts_count, uint16_t start_address, uint16_t space_size);

	/*
		Получить информацию об аккаунте с требуемым идентификатором
	*/
	bool GetAccount(uint32_t ID, Account& account) const;

	/*
		Изменить информацию аккаунта
	*/
	bool SetAccount(uint32_t ID, Account& account);

	/*
		Получить информацию о текущем аккаунте
	*/
	bool GetCurrentAccount(Account& account) const;

	/*
		Изменить информацию текущего аккаунта
	*/
	bool SetCurrentAccount(Account& account);

	/*
		Получить пространство аутентифицированного пользователя
	*/
	bool GetCurrentSpace(NamedSpace*& account_space);

	/*
		Получить объект с ограниченным доступом к пользовательскому пространству
	*/
	AuthAccessor MakeAccessor();

};

/*
	Объект с ограниченным доступом к пользовательскому пространству, 
	который можно передавать в другие объекты
*/
class AuthAccessor
{
protected:
	AuthHandler& auth_handler;

public:
	explicit AuthAccessor(AuthHandler& auth_handler);

	/*
		Получить пространство аутентифицированного пользователя
	*/
	bool GetCurrentSpace(NamedSpace*& account_space);
};


#endif /* INC_UTILS_HANDLE_COMMANDLINE_FROM_UART_HPP_ */
