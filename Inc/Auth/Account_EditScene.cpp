#include "Account_EditScene.hpp"

#include <Auth/AuthHandler.hpp>
#include <SceneManager/SceneManager.hpp>

void Account_EditScene::OnApply()
{
	// TODO: Обработка ошибок
	Account account;
	edit_account.avatar.Get(account.Avatar);
	edit_account.name.Get(account.Name);

	AuthHandler::Instance().SetCurrentAccount(account);
	SceneManager::Instance().Return();
}

void Account_EditScene::OnCancel()
{
	SceneManager::Instance().Return();
}

#include <Bitmaps/Avatars.hpp>
#include <Auth/AuthScene.hpp>

#include <UI/BitmapSettingUI.hpp>
#include <UI/ButtonSettingUI.hpp>
#include <UI/TextSettingUI.hpp>

#include <Graphics/DefaultFont.hpp>


Account_EditScene::Account_EditScene()
	: settings{ "", &CommonAllocator }
{
	settings.AddSetting<BitmapEditableSettingUI<uint32_t>>("Аватар", Point2_i{16, 12}, &edit_account.avatar, GetAvatars());
	settings.AddSetting<TextEditableSettingUI<10>>("Имя", Point2_i{16, 32}, &edit_account.name, true);
	settings.AddSetting<ButtonCallInstanceSettingUI<Account_EditScene>>("Сохранить", Point2_i{32, 47}, this, &Account_EditScene::OnApply);
	settings.AddSetting<ButtonCallInstanceSettingUI<Account_EditScene>>("Отменить", Point2_i{36, 54}, this, &Account_EditScene::OnCancel);
	settings.SetFont(&Default_Font_7x7_small);

	settings.Enable();
	AddObject(&settings);
}

bool Account_EditScene::ProcessInput(const AppEvent& event)
{
	if (IsSystemReturnEvent(event))
	{
		SceneManager::Instance().Return();
		return true;
	}

	return false;
}

void Account_EditScene::OnShow()
{
	Account account;
	if (AuthHandler::Instance().GetCurrentAccount(account))
	{
		edit_account.avatar.Set(account.Avatar);
		edit_account.name.Set(account.Name);
	}
	else
	{
		edit_account.avatar.Set(0);
		edit_account.name.Set("");
	}
}
