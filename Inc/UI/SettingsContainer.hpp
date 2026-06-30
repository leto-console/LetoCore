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

public:
	SettingsContainer(const StaticText32& title, IAllocator* allocator)
		: title{ title }, allocator{ allocator }
	{
	}

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
			if (auto_alignment)
			{
				settings[i]->SetPosition({0, y});
				y += settings[i]->GetHeight();
			}

			if (settings[i]->IsActive())
			{
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

		if (IsSystemNextEvent(event, true))
		{
			NextSetting();
			return true;
		}
		else if (IsSystemPrevEvent(event, true))
		{
			PrevSetting();
			return true;
		}
		else if (IsSystemEnterEvent(event, true))
		{
			return true;
		}

		return false;
	}

	void Loop() override 
	{ 
		RefreshPosition();
		for (ISettingUI* setting : settings)
			setting->Loop();
	}
};

#endif
