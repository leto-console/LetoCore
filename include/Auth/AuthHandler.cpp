#include <Auth/AuthHandler.hpp>

#include <System/SystemAllocator.hpp>
#include <System/EternalAllocator.hpp>
#include "AuthHandler.hpp"

bool AuthHandler::GetIndexByID(uint32_t ID, uint32_t& idx) const
{
	Account account;

	for (idx = 0; idx < accounts_count; ++idx)
	{
		if (accounts[idx] && accounts[idx]->Get(account))
		{
			if (account.ID == ID)
				return true;
		}
	}

	return false;
}

bool AuthHandler::CanCreateAccount(uint16_t* index) const
{
	for (uint16_t i = 0; i < accounts_count; ++i)
	{
		StoredDataCell<Account>* cell = accounts[i];
		Account acc;
		if (cell && cell->Get(acc) && acc.ID == 0)
		{
			if (index) *index = i;
			return true;
		}
	}

	return false;
}

bool AuthHandler::CreateAccount(const StaticText32& name, uint32_t avatar, uint32_t& ID)
{
	uint16_t index;
	if (!CanCreateAccount(&index))
		return false;

	if (!accounts[index] || current_space)
		return false;

	if (!new_ID_cell || !new_ID_cell->Get(ID))
		return false;

	current_space = SystemAllocator.Make<NamedSpace>(accounts[index]->GetAddress() + sizeof(Account), space_size - sizeof(Account), storage);
	if (!current_space) return false;

	current_space->Clear();

	Account account{};
	account.ID = ID;
	account.Avatar = avatar;
	account.Name = name;

	new_ID_cell->Set(ID + 1);
	cur_ID_cell->Set(ID);

	accounts[index]->Set(account);

	return true;
}

bool AuthHandler::DeleteAccount(uint32_t ID)
{
	uint32_t idx;

	if (!GetIndexByID(ID, idx))
		return false;

	if (!accounts[idx])
		return false;

	Account account{};
	accounts[idx]->Set(account);
	return true;
}

StaticListView<Account> AuthHandler::GetAccounts() const
{
	last_get_accounts.Clear();

	for (int idx = 0; idx < accounts_count; ++idx)
	{
		Account account;
		if (accounts[idx] && accounts[idx]->Get(account))
		{
			if (account.ID != 0)
				last_get_accounts.Push(account);
		}
	}

	return last_get_accounts;
}

bool AuthHandler::Login(uint32_t ID)
{
	uint32_t idx;
	if (!cur_ID_cell || !GetIndexByID(ID, idx) || current_space)
		return false;

	current_space = SystemAllocator.Make<NamedSpace>(accounts[idx]->GetAddress() + sizeof(Account), space_size - sizeof(Account), storage);
	if (!current_space) return false;

	current_space->Scan();
	cur_ID_cell->Set(ID);

	return true;
}

void AuthHandler::Logout()
{
	current_space = nullptr;
	// cur_ID_cell->Set(0);
}

void AuthHandler::Init(uint16_t auth_address, Storage& auth_storage, uint16_t auth_accounts_count, uint16_t start_address, uint16_t space_size)
{
	if (auth_accounts_count > (uint16_t) accounts.Size())
		auth_accounts_count = (uint16_t) accounts.Size();

	this->storage = &auth_storage;
	this->accounts_count = auth_accounts_count;
	this->space_size = space_size;

	if (!cur_ID_cell)
	{
		cur_ID_cell = EternalAllocator.Make<StoredDataCell<uint32_t>>(auth_address, &auth_storage);
		uint32_t ID;
		if (cur_ID_cell->Get(ID) && (ID == UINT32_MAX))
			cur_ID_cell->Set(0);
	}

	if (!new_ID_cell)
	{
		new_ID_cell = EternalAllocator.Make<StoredDataCell<uint32_t>>(auth_address + 4, &auth_storage);
		uint32_t ID;
		if (new_ID_cell->Get(ID) && (ID == 0 || ID == UINT32_MAX))
			new_ID_cell->Set(1);
	}

	for (uint16_t i = 0; i < auth_accounts_count; ++i)
	{
		accounts[i + 1] = EternalAllocator.Make<StoredDataCell<Account>>(start_address + i * space_size, storage);
	}
}


bool AuthHandler::GetAccount(uint32_t ID, Account& account) const
{
	uint32_t idx;

	if (!GetIndexByID(ID, idx))
		return false;

	if (!accounts[idx] || !accounts[idx]->Get(account))
		return false;

	return true;
}

bool AuthHandler::SetAccount(uint32_t ID, Account& account)
{
	uint32_t idx;

	if (!GetIndexByID(ID, idx))
		return false;

	if (!accounts[idx])
		return false;

	account.ID = ID;
	accounts[idx]->Set(account);
	return true;
}

bool AuthHandler::GetCurrentAccount(Account& account) const
{
	uint32_t cur_ID;

	if (!cur_ID_cell || !cur_ID_cell->Get(cur_ID) || cur_ID == 0 || !GetAccount(cur_ID, account))
		return false;

	return true;
}

bool AuthHandler::SetCurrentAccount(Account& account)
{
	uint32_t cur_ID;

	if (!cur_ID_cell || !cur_ID_cell->Get(cur_ID) || cur_ID == 0)
		return false;

	SetAccount(cur_ID, account);
	return true;
}

bool AuthHandler::GetCurrentSpace(NamedSpace*& account_space)
{
	uint32_t cur_ID, cur_idx;

	if (!cur_ID_cell || !cur_ID_cell->Get(cur_ID) || cur_ID == 0 || !GetIndexByID(cur_ID, cur_idx))
		return false;

	account_space = current_space;
	return true;
}

AuthAccessor AuthHandler::MakeAccessor()
{
	return AuthAccessor(*this);
}

// ----------------------------------------------------------------------------------------------------

AuthAccessor::AuthAccessor(AuthHandler& auth_handler)
	: auth_handler{ auth_handler }
{
}

bool AuthAccessor::GetCurrentSpace(NamedSpace*& account_space)
{
	return auth_handler.GetCurrentSpace(account_space);
}
