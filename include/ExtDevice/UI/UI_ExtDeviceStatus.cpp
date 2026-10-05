#include "UI_ExtDeviceStatus.hpp"

#include <Input/SystemInputID.hpp>
#include <LetoFunctions/Font.hpp>
#include <LetoFunctions/Draw.hpp>
#include <LetoFunctions/Text.hpp>

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
    font = leto::font::FindFont(6, LFV1_BASE_NORMAL);
    SetDevice(device);
}

void UI_ExtDeviceStatus::SetDevice(ExtDevice *device) { this->device = device; }
void UI_ExtDeviceStatus::SetFont(const LetoFont_V1 *font) { this->font = font; }

void UI_ExtDeviceStatus::Draw(IScreen& screen, Point2_i offset)
{
    if (!device)
    {
        leto::graphics::DrawText(IScreen::ToHandle(&screen), position + offset, "Device N/A", font, IndigoColor, BlackColor);
        return;
    }

    ExtDeviceStatus status = device->GetStatus(true);

    if ((uint32_t) status >= sizeof(ui_status) / sizeof(ui_status[0]))
        return;

    int name_offset = leto::graphics::GetTextWidth(device->GetName(), font) + leto::graphics::GetTextWidth(" ", font);
    
    leto::graphics::DrawText(IScreen::ToHandle(&screen), position + offset, device->GetName(), font, WhiteColor, BlackColor);

    switch (mode)
    {
    case 0:
    {
        const StaticText32& txt_status = ui_status[(uint32_t) status].text;
        RGBColor color = ui_status[(uint32_t) status].color;
        leto::graphics::DrawText(IScreen::ToHandle(&screen), position + offset + Point2_i{ name_offset, 0 }, txt_status, font, color, BlackColor);
        break;
    }
    case 1:
    {
        char txt[64]{};
        leto::text::FormatText(txt, sizeof(txt), "{ #00ffff }t:{ # }%-5d{ #00ffff }i:{ # }%-5d", device->GetAverageTimeTick(), device->GetAverageTimeInit());
        leto::graphics::DrawText(IScreen::ToHandle(&screen), position + offset + Point2_i{ name_offset, 0 }, txt, font, WhiteColor, BlackColor);
        break;
    }
    case 2:
    {
        char txt[64]{};
        leto::text::FormatText(txt, sizeof(txt), "{ #00ffff }p:{ # }%-5d", device->GetAverageTimePing()); // { #00ffff }i:{ # }%-5d
        leto::graphics::DrawText(IScreen::ToHandle(&screen), position + offset + Point2_i{ name_offset, 0 }, txt, font, WhiteColor, BlackColor);
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
