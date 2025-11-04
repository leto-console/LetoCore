#include <Auth/AuthHandler.hpp>

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

	if (!accounts[index] || !spaces[index])
		return false;

	if (!new_ID_cell || !new_ID_cell->Get(ID))
		return false;

	Account account;
	account.ID = ID;
	account.Avatar = avatar;
	account.Name = name;

	new_ID_cell->Set(ID + 1);

	accounts[index]->Set(account);
	spaces[index]->Clear();

	return true;
}

bool AuthHandler::DeleteAccount(uint32_t ID)
{
	uint32_t idx;

	if (!GetIndexByID(ID, idx))
		return false;

	if (!accounts[idx] || !spaces[idx])
		return false;

	Account account{};
	accounts[idx]->Set(account);
	spaces[idx]->Clear();
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
	if (!cur_ID_cell || !GetIndexByID(ID, idx) || !spaces[idx])
		return false;

	spaces[idx]->Scan();
	cur_ID_cell->Set(ID);

	return true;
}

void AuthHandler::Init(uint16_t auth_address, Storage& auth_storage, uint16_t auth_accounts_count)
{
	if (auth_accounts_count > (uint16_t) accounts.Size())
		auth_accounts_count = (uint16_t) accounts.Size();

	this->storage = &auth_storage;
	this->accounts_count = auth_accounts_count;

	if (!cur_ID_cell)
	{
		cur_ID_cell = new StoredDataCell<uint32_t>(auth_address, auth_storage);
		uint32_t ID;
		if (cur_ID_cell->Get(ID) && (ID == UINT32_MAX))
			cur_ID_cell->Set(0);
	}

	if (!new_ID_cell)
	{
		new_ID_cell = new StoredDataCell<uint32_t>(auth_address + 4, auth_storage);
		uint32_t ID;
		if (new_ID_cell->Get(ID) && (ID == 0 || ID == UINT32_MAX))
			new_ID_cell->Set(1);
	}
}

void AuthHandler::InitAccountSpace(uint16_t index, uint16_t start_address, uint16_t space_size)
{
	if (index >= accounts_count || index == 0)
		return;

	if (!accounts[index])
		accounts[index] = new StoredDataCell<Account>(start_address, *storage);

	if (!spaces[index])
		spaces[index] = new NamedSpace(start_address + sizeof(Account), space_size - sizeof(Account), *storage);
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

	account_space = spaces[cur_idx];
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
