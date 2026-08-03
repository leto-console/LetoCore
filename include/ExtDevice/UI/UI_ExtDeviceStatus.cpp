#include "UI_ExtDeviceStatus.hpp"

#include <DrawFunctions/DrawText.hpp>
#include <Input/SystemInputID.hpp>

#include <cstdio>

struct UI_ExtDeviceStatusDef
{
    StaticText32 text;
    RGBColor color;
};

static const UI_ExtDeviceStatusDef ui_status[]
{
    { "НЕОПРЕД", GrayColor },
    { "ЗАНЯТ", DeepOrangeColor },
    { "ОТКЛЮЧ", YellowColor },
    { "BAD_INIT", CyanColor },
    { "INIT_NG", PurpleColor },
    { "ПОДКЛЮЧ", DeepOrangeColor },
    { "ОК", GreenColor },
};

UI_ExtDeviceStatus::UI_ExtDeviceStatus(ExtDevice* device)
{
    SetDevice(device);
}

void UI_ExtDeviceStatus::SetDevice(ExtDevice *device) { this->device = device; }
void UI_ExtDeviceStatus::SetFont(const IFont *font) { this->font = font; }

void UI_ExtDeviceStatus::Draw(IScreen& screen, Point2_i offset)
{
    if (!device)
    {
        DrawFunctions::DrawText(screen, position + offset, "Device N/A", IndigoColor, BlackColor, false, font);
        return;
    }

    ExtDeviceStatus status = device->GetStatus(true);

    if ((uint32_t) status >= sizeof(ui_status) / sizeof(ui_status[0]))
        return;

    if (!font) font = DrawFunctions::GetDefaultFont();
    int name_offset = DrawFunctions::TextWidth(device->GetName(), font) + font->GetWidth();
    DrawFunctions::DrawText(screen, position + offset, device->GetName(), WhiteColor, BlackColor, false, font);

    switch (mode)
    {
    case 0:
    {
        const StaticText32& txt_status = ui_status[(uint32_t) status].text;
        RGBColor color = ui_status[(uint32_t) status].color;
        DrawFunctions::DrawText(screen, position + offset + Point2_i{ name_offset, 0 }, txt_status, color, BlackColor, false, font);
        break;
    }
    case 1:
    {
        char txt[64];
        snprintf(txt, sizeof(txt), "{ #00ffff }t:{ # }%-5d{ #00ffff }i:{ # }%-5d", device->GetAverageTimeTick(), device->GetAverageTimeInit());
        DrawFunctions::DrawText(screen, position + offset + Point2_i{ name_offset, 0 }, txt, sizeof(txt), WhiteColor, BlackColor, false, font);
        break;
    }
    case 2:
    {
        char txt[64];
        snprintf(txt, sizeof(txt), "{ #00ffff }p:{ # }%-5d", device->GetAverageTimePing()); // { #00ffff }i:{ # }%-5d
        DrawFunctions::DrawText(screen, position + offset + Point2_i{ name_offset, 0 }, txt, sizeof(txt), WhiteColor, BlackColor, false, font);
        break;
    }
    default:
        break;
    }
}

bool UI_ExtDeviceStatus::ProcessInput(const AppEvent& event)
{
    if (!device) return false;

    if (IsSystemEnterEvent(event) && device->GetStatus() != ExtDeviceStatus::READY)
    {
        device->AsyncInit();
        return true;
    }
    else if (IsSystemAltEvent(event))
    {
        ++mode %= MODES_COUNT;
        return true;
    }

    return false;
}
