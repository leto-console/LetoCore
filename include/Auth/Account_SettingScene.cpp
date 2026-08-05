#include "Account_SettingScene.hpp"

#include <Graphics/DefaultFont.hpp>
#include <Input/SystemInputID.hpp>
#include <SceneManager/ISceneManager.hpp>
#include <Auth/AuthHandler.hpp>
#include <Bitmaps/Avatars.hpp>
#include <DrawFunctions/DrawRectangle.hpp>
#include <DrawFunctions/DrawBitmap.hpp>
#include <DrawFunctions/DrawText.hpp>

Account_SettingScene::Account_SettingScene(ISceneManager* scene_manager, uint32_t editID, uint32_t deleteID)
	: IScene{scene_manager}, editID{ editID }, deleteID{ deleteID }
{
	menu.InitBaseCatchers();
	menu.AppendMenuItem("ИЗМЕНИТЬ", editID);
	menu.AppendMenuItem("УДАЛИТЬ", deleteID);
	menu.SetStyle(MenuStyle::STYLE_3, &Default_Font_7x7_small);
	menu.SetHorizonAlignment(MenuHorizonAlignment::CENTER);
	menu.SetPosition({64, 40});
	menu.Enable();
	AddObject(&menu);
}

void Account_SettingScene::Draw(IScreen& screen)
{
	using namespace DrawFunctions;

	const BitmapData* data = GetAvatarByID(cur_account.avatar.GetOrDefault());
	if (data) DrawBitmap(screen, {12, 10}, *data, WhiteColor);

	int text_y = 10;
	text_y += (data->height / 2);
	text_y -= Default_Font_8x8.GetHeight() / 2;

	DrawRectangle(
		screen,
		{ 44, text_y - 1 },
		{ 44 + TextWidth(cur_account.name.GetOrDefault(), &Default_Font_8x8), text_y + Default_Font_8x8.GetHeight() },
		WhiteColor
	);

	DrawText(screen, { 44, text_y }, cur_account.name.GetOrDefault(), WhiteColor, BlackColor, true, &Default_Font_8x8);
}

bool Account_SettingScene::ProcessInput(const AppEvent& event)
{
	if (IsSystemEnterEvent(event))
	{
		scene_manager->SwitchScene(menu.GetCurrentParam());
	}

	if (IsSystemReturnEvent(event))
	{
		scene_manager->Return();
		return true;
	}

	return false;
}

void Account_SettingScene::OnShow()
{
	Account account;
	if (AuthHandler::Instance().GetCurrentAccount(account))
	{
		cur_account.avatar.Set(account.Avatar);
		cur_account.name.Set(account.Name);
	}
	else
	{
		scene_manager->Return();
	}
}

bool Account_SettingScene::Loop()
{
	return true;
}
