#include "TestUI.hpp"

#include "UI_Object.hpp"
#include "UI_Label.hpp"

#include <cstdlib>

#include <Time/Timer.hpp>

static float RandomFloat()
{
    return (float) rand() / (float)RAND_MAX;
}

template <typename Type>
static Type Random(Type a, Type b)
{
    float r = RandomFloat();
    if (r >= 1.0f)
        return (Type) (b - 1);
    return (Type) (r * (b - a)) + a;
}

static UI_Label_new* SetupLabel(UI_Label_new* label)
{
    const char* texts[]
    {
        "Привет!",
        "Как дела?",
        "Hello!",
        "How are you?",
    };

    int rand_txt = Random<int>(0, sizeof(texts)/sizeof(*texts));
    label->SetText(texts[rand_txt]);
    label->SetFont(leto_api_v1->Font->FindFont(6, LFT_V1_BASE_NORMAL));
    label->SetHorizonAlignment(UI_Label_HAlign::CENTER);
    label->SetVerticalAlignment(UI_Label_VAlign::CENTER);
    
    struct
    {
        RGBColor m, b;
    }
    pairs[6]
    {
        { RedColor, YellowColor },
        { RedColor, BlueColor },
        { BlueColor, YellowColor },

        { YellowColor, RedColor },
        { BlueColor, RedColor },
        { YellowColor, BlueColor },
    };
    
    int rand_clr = Random<int>(0, sizeof(pairs)/sizeof(*pairs));
    label->SetTextColor(pairs[rand_clr].m);
    label->SetBackroundColor(pairs[rand_clr].b);
    if (rand_clr % 2 == 0)
        label->Select(true);

    Point2_i pos;
    pos.x = Random<int>(0, 160);
    pos.y = Random<int>(0, 128);
    label->SetPosition(pos);

    label->Enable(true);
    return label;
}

class TestUI_Scene : public UI_Object
{
protected:
    UI_Label_new* label = nullptr;
    Timer rand_timer;

public:
    TestUI_Scene() : UI_Object{ {160, 128}, leto_api_v1->Globals->GetAllocator() }
    {
        //srand(0xC1A0C1A0);
        rand_timer.Start(50);
        Point2_i p{90, 20};

        for (int i = 0; i < 32; ++i)
            AddChild(SetupLabel(leto_new UI_Label_new(p)));
 
        Enable(true);
    }

    void ObjectDraw(LetoScreen_V1* screen) override
    {
    }

    void ObjectLoop() override
    {
        if (!rand_timer.Expired()) return;
        rand_timer.Start();
        
        uint8_t idx = 0;
        for (UI_Object* obj : childs)
        {
            Point2_i pos = obj->GetPosition();
            if (pos.x > 0 && pos.x < 160 &&
                pos.y > 0 && pos.y < 128)
            {
                int A = (idx / 8) + 2;
                pos.x += Random<int>(-A, A+1);
                pos.y += Random<int>(-A, A+1);
            }
            else
            {
                pos.x = Random<int>(0, 160);
                pos.y = Random<int>(0, 128);
            }
            obj->SetPosition(pos);
            idx++;
        }
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
