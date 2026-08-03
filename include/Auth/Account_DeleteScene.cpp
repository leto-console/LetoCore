#include "Account_DeleteScene.hpp"

#include <Auth/AuthHandler.hpp>
#include <SceneManager/SceneManager.hpp>

#include <Bitmaps/Avatars.hpp>
#include <Auth/AuthScene.hpp>

#include <Graphics/DefaultFont.hpp>

#include <Auth/Account.hpp>
#include <System/SystemMode.hpp>

#include <Input/SystemInputID.hpp>


Account_DeleteScene::Account_DeleteScene()
{
	delete_question.SetText(0, "ВНИМАНИЕ!");
	delete_question.SetText(1, "УДАЛИТЬ АККАУНТ");

	delete_question.AppendMenuItem("ОТМЕНИТЬ", false);
	delete_question.AppendMenuItem("УДАЛИТЬ", true);
}

bool Account_DeleteScene::ProcessInput(const AppEvent& event)
{
	if (delete_question.ProcessInput(event))
		return true;

	if (IsSystemReturnEvent(event))
	{
		SceneManager::Instance().Return();
		return true;
	}

	return false;
}

void Account_DeleteScene::Draw(IScreen& screen)
{
	delete_question.Draw(screen, {64, 0});
}

bool Account_DeleteScene::Loop()
{
	bool answer;
	if (delete_question.IsResultReady(answer))
	{
		Account account;
		if (answer && AuthHandler::Instance().GetCurrentAccount(account))
		{
			AuthHandler::Instance().DeleteAccount(account.ID);
			SetSystemMode(SystemMode::AUTH);
		}
		else
		{
			SceneManager::Instance().Return();
		}
		delete_question.Disable();
	}
	return true;
}

void Account_DeleteScene::OnShow()
{
	delete_question.ResetCurrentID();

	Account account;
	if (AuthHandler::Instance().GetCurrentAccount(account))
	{
		StaticText32 text = account.Name;
		text += "?";
		delete_question.SetText(2, text);
		delete_question.Enable();
	}
	else
	{
		// TODO: или переключиться в режим авторизации?
		SceneManager::Instance().Return();
	}
}
