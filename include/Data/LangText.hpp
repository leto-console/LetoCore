/**
 * @file LangText.hpp
 * @date Oct 10, 2026
 * @author Rakhimov T.
 */

#ifndef INC_DATA_LANG_TEXT_HPP_
#define INC_DATA_LANG_TEXT_HPP_

#include <cstring>
#include <cstdint>
#include <initializer_list>

#include <Data/StaticText.hpp>
#include <LetoAPI_V1/LetoAPI_V1.h>

struct LangItem
{
	const char* text{};
	LetoLanguage_V1 lang{};

	constexpr LangItem() = default;

	constexpr LangItem(LetoLanguage_V1 lang, const char* text)
		: lang{ lang }, text{ text }
	{ }
};

namespace Translation
{
	constexpr LangItem RUS(const char* text) { return { LETO_LANG_V1_RUS, text }; }
};

template <size_t LangsCount>
class LangText
{
protected:
	LangItem lang_items[LangsCount]{};
	uint8_t cnt{};

	constexpr void AddLang(const LangItem& item)
	{
		if (item.lang == 0 || Translate(item.lang) != nullptr) return;
		if (cnt < LangsCount) 
        {
			lang_items[cnt] = item;
            cnt++;
        }
	}

public:
	constexpr LangText(const char* EN, std::initializer_list<LangItem> _langs = {})
	{
		AddLang({ LETO_LANG_V1_ENG, EN });
		for (const LangItem& lang : _langs)
		{
			AddLang(lang);
		}
	}

	constexpr const char* Translate(LetoLanguage_V1 lang) const
	{
        for (uint8_t i = 0; i < cnt; ++i)
		{
			if (lang_items[i].lang == lang)
				return lang_items[i].text;
		}
		return nullptr;
	}
	
	const char* Text() const
	{
        if (!leto_api_v1 || !leto_api_v1->Globals)
            return Translate(LETO_LANG_V1_ENG);

		LetoLanguage_V1 lang = leto_api_v1->Globals->GetSystemLanguage();
		const char* translate = Translate(lang);
		if (translate) return translate;

		return Translate(LETO_LANG_V1_ENG);
	}
};

#endif