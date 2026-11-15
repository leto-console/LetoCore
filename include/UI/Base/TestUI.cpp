#include "TestUI.hpp"

#include "UI_Object.hpp"
#include "UI_Label.hpp"

class TestUI_Scene : public UI_Object
{
protected:
    UI_Label_new* label = nullptr;
public:
    TestUI_Scene() : UI_Object{ {160, 128}, leto_api_v1->Globals->GetAllocator() }
    {
        label = leto_new UI_Label_new({160, 128});
        label->SetText("ПРИВЕТ!");
        label->SetFont(leto_api_v1->Font->FindFont(6, LFT_V1_BASE_NORMAL));
        label->SetHorizonAlignment(UI_Label_HAlign::CENTER);
        label->SetVerticalAlignment(UI_Label_VAlign::CENTER);
        label->SetTextColor(aRedColor);
        label->SetBackroundColor(aGreenColor);
        label->Enable(true);

        AddChild(label);
        Enable(true);
    }

    void ObjectDraw(LetoScreen_V1* screen) override
    {
        Point2_i size = label->GetSize();
        if (size.y > 20) size.y--;
        else label->SetColorInverse(true);
        label->SetSize(size);
    }

    void ObjectLoop() override
    {
    }
};
static TestUI_Scene* object = nullptr;

void TestUI_Init()
{
    object = leto_new TestUI_Scene;
}

void TestUI_DeInit()
{
    object = nullptr;
}

void TestUI_Draw(IScreen &screen)
{
    if (!object) return;
    object->Draw(IScreen::ToHandle(&screen));
}

void TestUI_OnEvent(const AppEvent &event)
{
    if (!object) return;
    object->OnEvent(event);
}

void TestUI_Loop()
{
    if (!object) return;
    object->Loop();
}
