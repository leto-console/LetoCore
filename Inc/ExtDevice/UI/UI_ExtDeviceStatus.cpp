#include "UI_ExtDeviceStatus.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <Input/SystemInputID.hpp>

struct UI_ExtDeviceStatusDef
{
    StaticText32 text;
    RGBColor color;
};

static const UI_ExtDeviceStatusDef ui_status[]
{
    { "НЕОПРЕД", GrayColor },
    { "ОК", GreenColor },
    { "ЗАНЯТ", DeepOrangeColor },
    { "ОТКЛЮЧ", YellowColor },
    { "BAD_INIT", CyanColor },
    { "INIT_NG", PurpleColor }
};

UI_ExtDeviceStatus::UI_ExtDeviceStatus(ExtDevice* device)
    : device{ device }, font{ }
{
}

void UI_ExtDeviceStatus::SetFont(IFont *font) { this->font = font; }

void UI_ExtDeviceStatus::Draw(IScreen& screen, Point2_i offset)
{
    if (!device)
    {
        DrawFunctions::DrawText(screen, position, "Device N/A", IndigoColor, BlackColor, false, font);
        return;
    }

    ExtDeviceStatus status = device->GetStatus(true);

    if ((uint32_t) status >= sizeof(ui_status) / sizeof(ui_status[0]))
        return;
    
    const StaticText32& txt_status = ui_status[(uint32_t) status].text;
    RGBColor color = ui_status[(uint32_t) status].color;

    if (!font) font = DrawFunctions::GetDefaultFont();
    int name_offset = DrawFunctions::TextWidth(device->GetName(), font) + font->GetWidth();
    DrawFunctions::DrawText(screen, position, device->GetName(), WhiteColor, BlackColor, false, font);
    DrawFunctions::DrawText(screen, position + Point2_i{ name_offset, 0 }, txt_status, color, BlackColor, false, font);
}

bool UI_ExtDeviceStatus::ProcessInput(const AppEvent& event)
{
    if (!device) return false;

    if (IsSystemEnterEvent(event) && device->GetStatus() != ExtDeviceStatus::READY)
    {
        device->AsyncInit();
        return true;
    }

    return false;
}
