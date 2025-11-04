#include "Account_CreateScene.hpp"

#include <cstring>


#include "UI/ISettingUI.hpp"
#include "UI/BitmapSettingUI.hpp"
#include "UI/ButtonSettingUI.hpp"
#include "UI/TextSettingUI.hpp"

#include <Bitmaps/Avatars.hpp>
#include <Auth/AuthScene.hpp>
#include <Auth/AuthHandler.hpp>
#include <System/SystemMode.hpp>

#include <Graphics/DefaultFont.hpp>
#include "Account_EditScene.hpp"
#include <SceneManager/SceneManager.hpp>

void Account_CreateScene::OnCreateAccount()
{
	uint32_t ID;
	if (!AuthHandler::Instance().CreateAccount(account.name.GetOrDefault(), account.avatar.GetOrDefault(), ID))
	{
		// Не удалось создать аккаунт
			// TODO: Обработка ошибок
		return;
	}
	AuthHandler::Instance().Login(ID);
	SetSystemMode(SystemMode::USER);
}

void Account_CreateScene::OnCancel()
{
	auth_scene.ShowSelectScene();
}

Account_CreateScene::Account_CreateScene(AuthScene& auth_scene)
	: auth_scene{ auth_scene }, settings{ "", &CommonAllocator }
{
	settings.AddSetting<BitmapEditableSettingUI<uint32_t>>("Аватар", Point2_i{16, 12}, &account.avatar, GetAvatars());
	settings.AddSetting<TextEditableSettingUI<10>>("Имя", Point2_i{16, 32}, &account.name, true);
	settings.AddSetting<ButtonCallInstanceSettingUI<Account_CreateScene>>("Создать", Point2_i{40, 47}, this, &Account_CreateScene::OnCreateAccount);
	settings.AddSetting<ButtonCallInstanceSettingUI<Account_CreateScene>>("Отменить", Point2_i{36, 54}, this, &Account_CreateScene::OnCancel);

	settings.SetFont(&Default_Font_7x7_small);

	settings.Enable();
	AddObject(&settings);
}

void Account_CreateScene::OnShow()
{
	account.avatar.Set(0);
	account.name.Set("");
}
