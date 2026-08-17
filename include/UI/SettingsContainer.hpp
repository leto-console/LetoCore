/**
 * @file SettingsContainer.npp
 * @date Mar 27, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_SETTINGS_CONTAINER_HPP_
#define INC_UI_SETTINGS_CONTAINER_HPP_

#include <UI/ISettingUI.hpp>
#include <Input/SystemInputID.hpp>
#include <Drawable/IDrawable.hpp>
#include <SceneManager/ISceneObject.hpp>

#include <Data/StaticText.hpp>
#include <Data/StaticList.hpp>
#include <Data/IAllocator.hpp>

#include <DrawFunctions/DrawText.hpp>
#include <Input/Catchers/ButtonCatcher.hpp>
#include <Input/Catchers/EncoderCatcher.hpp>

/**
 * @brief Контейнер настроек
 * 
 * Представляет собой `ISceneObject`, которые можно регистрировать в сценах
 */
class SettingsContainer : public ISceneObject
{
protected:
	StaticText32 title;
	StaticList<ISettingUI*, 32> settings{};
	const IFont* settings_font{};
	IAllocator* allocator;

	bool auto_alignment = false;
	int cur_setting{};

	ButtonCatcher<SettingsContainer> up_catcher, down_catcher, enter_catcher;
	EncoderCatcher<SettingsContainer> enc_catcher;

	void NextSetting()
	{
		if (!settings.Count()) return;

		int next_setting = cur_setting + 1;
		while (next_setting < settings.Count())
		{
			if (settings[next_setting]->IsActive())
				break;
			next_setting++;
		}
		if (next_setting == settings.Count())
			return;

		settings[cur_setting]->Select(false);
		cur_setting = next_setting;
		settings[cur_setting]->Select();
	}

	void PrevSetting()
	{
		if (!settings.Count()) return;

		int prev_setting = cur_setting - 1;
		while (prev_setting >= 0)
		{
			if (settings[prev_setting]->IsActive())
				break;
			prev_setting--;
		}
		if (prev_setting < 0)
			return;

		settings[cur_setting]->Select(false);
		cur_setting = prev_setting;
		settings[cur_setting]->Select();
		return;
	}

	void Enter() {  }

	void Rotate(bool left)
	{
		if (left) PrevSetting();
		else      NextSetting();
	}

public:
	SettingsContainer(const StaticText32& title, IAllocator* allocator)
		: title{ title }, allocator{ allocator },
		up_catcher{ this, &SettingsContainer::PrevSetting },
		down_catcher{ this, &SettingsContainer::NextSetting },
		enter_catcher{ this, &SettingsContainer::Enter },
		enc_catcher{ this, &SettingsContainer::Rotate }
	{
		InitBaseCatchers();
	}

	void ButtonCatchUp(uint8_t button_id)
	{
		up_catcher.Catch(button_id, BCM_SINGLE_PRESS | BCM_MULTI_HOLD);
		up_catcher.SetHoldTime(200, 100);
	}

	void ButtonCatchDown(uint8_t button_id)
	{
		down_catcher.Catch(button_id, BCM_SINGLE_PRESS | BCM_MULTI_HOLD);
		down_catcher.SetHoldTime(200, 100);
	}

	void ButtonCatchEnter(uint8_t button_id)
	{
		enter_catcher.Catch(button_id, BCM_SINGLE_PRESS);
	}

	void EncoderCatch(uint8_t encoder_id)
	{
		enc_catcher.Catch(encoder_id, ECM_ROTATE);
	}

	void InitBaseCatchers()
	{
		ButtonCatchUp(SYSTEM_BTN_UP);
		ButtonCatchDown(SYSTEM_BTN_DOWN);
		ButtonCatchEnter(SYSTEM_BTN_ENTER);
		ButtonCatchEnter(SYSTEM_BTN_RIGHT);
		EncoderCatch(SYSTEM_ENC_MAIN);
	}

	int GetCurrentSettingIdx() const { return cur_setting; }

	void AddSetting(ISettingUI* setting)
	{
		settings.Push(setting);
	}

	template <typename Setting, typename... Args>
	Setting* AddSetting(Args... args)
	{
		static_assert(std::is_base_of<ISettingUI, Setting>::value);
		Setting* setting = allocator->Make<Setting>(std::forward<Args>(args)...);
		settings.Push(setting);
		return setting;
	}

	void SetFont(const IFont* settings_font)
	{
		this->settings_font = settings_font;
		for (ISettingUI* setting : settings)
			setting->SetFont(settings_font);
	}

	void RefreshPosition()
	{
		if (settings.Count() && !settings[cur_setting]->IsActive())
		{
			settings[cur_setting]->Capture(false);
			NextSetting();
			PrevSetting();
		}
		for (int i = 0; i < settings.Count(); ++i)
			this->settings[i]->Select(i == cur_setting);
	}

	void UpdateCurrentValue()
	{
		for (ISettingUI* setting : settings)
			setting->UpdateCurrentValue();
	}

	void OnShow() override
	{
		if (settings.Count() > 0)
		{
			Point2_i pos = settings[0]->GetPosition();
			if (pos.x < 0 || pos.y < 0)
				auto_alignment = true;
		}
		
		cur_setting = 0;
		UpdateCurrentValue();
		RefreshPosition();
	}

	void Draw(IScreen& screen, Point2_i offset = {}) override
	{
		if (!title.Empty())
			DrawFunctions::DrawText(screen, {0, 0}, title);

		for (int i = 0, y = settings_font ? settings_font->GetHeight() : 8; i < settings.Count(); ++i)
		{
			if (settings[i]->IsActive())
			{
				if (auto_alignment)
				{
					settings[i]->SetPosition({0, y});
					y += settings[i]->GetHeight();
				}

				settings[i]->Draw(screen);
			}
		}
	}

	bool ProcessInput(const AppEvent& event) override
	{
		for (ISettingUI* setting : settings)
		{
			if (setting->IsActive() &&
				setting->ProcessInput(event))
				return true;
		}

		if (up_catcher.ProcessInput(event) ||
			down_catcher.ProcessInput(event) ||
			enter_catcher.ProcessInput(event) ||
			enc_catcher.ProcessInput(event))
			return true;

		return false;
	}

	void Loop() override 
	{ 
		up_catcher.Loop();
		down_catcher.Loop();
		enter_catcher.Loop();
		enc_catcher.Loop();

		RefreshPosition();
		for (ISettingUI* setting : settings)
			setting->Loop();
	}
};

#endif
