#include "SystemTestScene.hpp"

#include <Input/ButtonEvent.hpp>
#include <DrawFunctions/DrawLine.hpp>
#include <SceneManager/ISceneManager.hpp>

static void DrawPallete(IScreen& screen)
{
    const uint8_t PARTS_COUNT = 6;
    int part_size = screen.Width() / PARTS_COUNT;
    int screen_height = screen.Height();
    RGBColor color;

    for (uint8_t part = 0; part < PARTS_COUNT; ++part)
    {
        for (int i = 0; i < part_size; ++i)
        {
            uint16_t k = i * 0x100 / part_size;

            switch (part)
            {
            case 0:
                color.R = 0xFF;
                color.G = k;
                color.B = 0x00;
                break;
            case 1:
                color.R = 0xFF - k;
                color.G = 0xFF;
                color.B = 0x00;
                break;
            case 2:
                color.R = 0x00;
                color.G = 0xFF;
                color.B = k;
                break;
            case 3:
                color.R = 0x00;
                color.G = 0xFF - k;
                color.B = 0xFF;
                break;
            case 4:
                color.R = k;
                color.G = 0x00;
                color.B = 0xFF;
                break;
            case 5:
                color.R = 0xFF;
                color.G = 0x00;
                color.B = 0xFF - k;
                break;
            default:
                break;
            }

            Point2_i point_a { part * part_size + i, 0 };
            Point2_i point_b = point_a;
            point_b.y = screen_height;

            DrawFunctions::DrawLine(screen, point_a, point_b, color);
        }      
    }
}

static void DrawGray(IScreen& screen)
{
    int screen_height = screen.Height();
    int screen_width = screen.Width();
    RGBColor color;

    for (int x = 0; x < screen_width; ++x)
    {
        uint16_t k = x * 0x100 / screen_width;

        color.R = color.G = color.B = k;

        Point2_i point_a { x, 0 };
        Point2_i point_b = point_a;
        point_b.y = screen_height;

        DrawFunctions::DrawLine(screen, point_a, point_b, color);
    }      
}

SystemTestScene::SystemTestScene(ISceneManager* scene_manager) : IScene{scene_manager}
{
    stage = STAGE_COLOR_R;
}

void SystemTestScene::Draw(IScreen &screen)
{
    switch (stage)
    {
    case STAGE_COLOR_R:
        screen.FillScreen(aRedColor);
        break;
    case STAGE_COLOR_G:
        screen.FillScreen(aGreenColor);
        break;
    case STAGE_COLOR_B:
        screen.FillScreen(aBlueColor);
        break;
    case STAGE_COLOR_PALLETE:
        DrawPallete(screen);
        break;
    case STAGE_COLOR_GRAY:
        DrawGray(screen);
        /// TODO
        break;
    default:
        break;
    }
}

bool SystemTestScene::Loop()
{
    if (stage == STAGE_DONE)
        scene_manager->Return();

    return true;
}

bool SystemTestScene::ProcessInput(const AppEvent &event)
{
    if (ButtonEvent::IsPressed(event))
    {
        switch (stage)
        {
        case STAGE_COLOR_R:
            stage = STAGE_COLOR_G;
            break;
        case STAGE_COLOR_G:
            stage = STAGE_COLOR_B;
            break;
        case STAGE_COLOR_B:
            stage = STAGE_COLOR_PALLETE;
            break;
        case STAGE_COLOR_PALLETE:
            stage = STAGE_COLOR_GRAY;
            break;
        case STAGE_COLOR_GRAY:
            stage = STAGE_DONE;
            break;
        default:
            break;
        }
    }

    return true;
}
