#include "SystemLanguage.hpp"

static IDataCell<LetoLanguage_V1>* SystemLanguageCell{};

void InitSystemLanguageCell(IDataCell<LetoLanguage_V1> *cell)
{
    if (!cell) return;
    
    SystemLanguageCell = cell;
    if (SystemLanguageCell->GetOrDefault() == LETO_LANG_V1_NONE)
    {
        SystemLanguageCell->Set(LETO_LANG_V1_ENG);
    }
}

void SetSystemLanguage(LetoLanguage_V1 language)
{
    if (language == LETO_LANG_V1_NONE || language >= _LETO_LANG_V1_COUNT) return;
    
    if (SystemLanguageCell) 
    {
        SystemLanguageCell->Set(language);
    }
}

LetoLanguage_V1 GetSystemLanguage()
{
    if (SystemLanguageCell)
    {
        LetoLanguage_V1 lang = SystemLanguageCell->GetOrDefault();
        if (lang == LETO_LANG_V1_NONE || lang >= _LETO_LANG_V1_COUNT)
        {
            return LETO_LANG_V1_ENG; 
        }
        return lang;
    }
    
    return LETO_LANG_V1_ENG; 
}
