#include <Auth/AuthScene.hpp>

#include <Graphics/DefaultFont.hpp>
#include <Input/SystemInputID.hpp>
#include <System/SystemMode.hpp>

#include <Auth/AuthHandler.hpp>
#include <Bitmaps/Avatars.hpp>
#include <Bitmaps/Bitmaps.h>
#include <DrawFunctions/DrawBitmap.hpp>
#include <DrawFunctions/DrawText.hpp>

enum
{
	_ADMIN,
	_CREATE,
	_USER_START
};

AuthScene::AuthScene() :
	menu{ 5, {40, 30} },
	create_scene{ *this }
{
	menu.SetStyle(MenuStyle::STYLE_2, &Default_Font_7x7);
	menu.SetVerticalAlignment(MenuVerticalAlignment::CENTER);
	menu.Enable();
	AddObject(&menu);
}

void AuthScene::ShowSelectScene()
{
	if (current_subscene)
		current_subscene->OnHide();
	current_subscene = nullptr;
	menu.Enable();
	if (current_subscene)
		current_subscene->MainOnShow();
}

void AuthScene::ShowCreateScene()
{
	if (current_subscene)
		current_subscene->OnHide();
	current_subscene = &create_scene;
	menu.Disable();
	if (current_subscene)
		current_subscene->MainOnShow();
}

void AuthScene::OnShow()
{
	menu.Clear();
	for (const Account& account : AuthHandler::Instance().GetAccounts())
	{
		menu.AppendMenuItem(account.Name, _USER_START + account.ID);
	}

	menu.AppendMenuItem("ADMIN", _ADMIN);

	if (AuthHandler::Instance().CanCreateAccount())
	{
		menu.AppendMenuItem("СОЗДАТЬ", _CREATE);
	}
}

void AuthScene::Draw(IScreen& screen)
{
	using namespace DrawFunctions;

	if (current_subscene)
	{
		current_subscene->MainDraw(screen);
		return;
	}

	Point2_i icon_point{10, 24};

	if (menu.GetCurrentParam() > _USER_START)
	{
		StaticText32 text = "ВОЙТИ:";
		DrawText(screen, { 48, 5 }, text, WhiteColor, BlackColor, false, &Default_Font_7x7);

		Account account;
		AuthHandler::Instance().GetAccount(menu.GetCurrentParam() - _USER_START, account);
		
		BitmapData data;
		if (GetAvatarByID(account.Avatar, data))
			DrawBitmap(screen, icon_point, data, WhiteColor);
	}
	else if (menu.GetCurrentParam() == _ADMIN)
	{
		DrawBitmap(screen, icon_point, BM_Crown, WhiteColor);
	}
	else if (menu.GetCurrentParam() == _CREATE)
	{
		DrawBitmap(screen, icon_point, BM_Create, WhiteColor);
	}
}

bool AuthScene::ProcessInput(const AppEvent& event)
{
	if (current_subscene)
		return current_subscene->MainProcessInput(event);

	if (IsSystemEnterEvent(event))
	{
		if (menu.GetCurrentParam() > _USER_START)
		{
			AuthHandler::Instance().Login(menu.GetCurrentParam() - _USER_START);
			SetSystemMode(SystemMode::USER);
		}
		else if (menu.GetCurrentParam() == _ADMIN)
		{
			SetSystemMode(SystemMode::ADMIN);
		}
		else if (menu.GetCurrentParam() == _CREATE)
		{
			ShowCreateScene();
		}
		return true;
	}
	return false;
}

bool AuthScene::Loop()
{
	if (current_subscene)
		return current_subscene->MainLoop();
	return false;
}
