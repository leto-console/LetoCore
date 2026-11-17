/**
 * @file UI_Container.hpp
 * @date Nov 17, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>

#include <Input/CatchersNew/ButtonCatcher.hpp>
#include <Input/SystemInputID.hpp>
#include <UI/Base/UI_Object.hpp>

NEW_LETO_UI_BEGIN

enum class UI_ContainerType
{
	Vertical = 1,	///< Align to begin of layer
	Horizontal,		///< Align to center of layer
};

enum class UI_ContainerAlign
{
	Begin = 1,		///< Align to begin of layer
	Center,			///< Align to center of layer
	End				///< Align to end of layer
};

class UI_Container : public UI_Object
{
public:
	UI_Container(UI_ContainerType type, Point2_i size, const LetoAllocator_V1* allocator = nullptr)
        : UI_Object{ size, allocator }, type{ type }, layers{ allocator },
		prev_catcher{ this, &UI_Container::OnPrev },
		next_catcher{ this, &UI_Container::OnNext }
	{ 
		AddChild(&prev_catcher);
		AddChild(&next_catcher);
	}
	UI_Container(UI_ContainerType type, Point2_i size, UI_Object& parent)
        : UI_Container{type, size, parent.GetAllocator()}
	{ }

	void InitBaseCatchers()
	{
		ButtonCatchPrev(type == UI_ContainerType::Horizontal ? SYSTEM_BTN_LEFT : SYSTEM_BTN_UP);
		ButtonCatchNext(type == UI_ContainerType::Horizontal ? SYSTEM_BTN_RIGHT : SYSTEM_BTN_DOWN);
	}
	
	void SetAlign(UI_ContainerAlign align)
	{
		if (this->align != align)
		{
            this->align = align;
			Invalidate();
		}
	}
	UI_ContainerAlign GetAlign() const { return align; }
	
	void ButtonCatchPrev(uint8_t button_id) 
	{ 
		prev_catcher.Catch(button_id, BCM_SINGLE_PRESS | BCM_HOLD_REPETITION);
		prev_catcher.SetHoldTime(200, 100);
	}
	void ButtonCatchNext(uint8_t button_id) 
	{ 
		next_catcher.Catch(button_id, BCM_SINGLE_PRESS | BCM_HOLD_REPETITION);
		next_catcher.SetHoldTime(200, 100);
	}
		
	void AddLayer(UI_Object* layer)
	{
		layers.push_back(layer);
		RefreshLayout();
		UpdateLayer();
		Invalidate();
	}
	
	UI_Container& operator<<(UI_Object* layer)
	{
		AddLayer(layer);
        return *this;
	}

protected:
	ButtonCatcher<UI_Container> prev_catcher, next_catcher;
	
	uint32_t layer_idx = 0;
	UI_Object* layer_ptr = nullptr;	
	
	int& GetAxis(Point2_i& p) const
	{
		return type == UI_ContainerType::Vertical ? p.y : p.x;
	}
	
	int GetAxis(const Point2_i& p) const
	{
		return type == UI_ContainerType::Vertical ? p.y : p.x;
	}


	//                 [--]
	//               [--|---]
	//            [--|---|-----]
	
	void RefreshLayout()
	{
		// Пересобрать сетку (при изменении выравнивания или ещё чего-нибудь)
		uint32_t length = 0;
		for (UI_Object* layer : layers)
		{
			length += GetAxis(layer->GetSize());
			length += margin;
		}

		Point2_i align_offset{};
		switch (align)
		{
		case UI_ContainerAlign::Center:
			GetAxis(align_offset) = (GetAxis(GetSize()) - length) / 2;
			break;
		case UI_ContainerAlign::End:
			GetAxis(align_offset) = GetAxis(GetSize()) - length;
			break;
		};

		Point2_i p = GetPosition() + align_offset;
		for (UI_Object* layer : layers)
		{
			layer->SetPosition(p);
			
			GetAxis(p) += GetAxis(layer->GetSize());
			GetAxis(p) += margin;
		}
	}
	
	void ObjectDraw(LetoScreen_V1* screen) override
	{
		for (UI_Object* layer : layers)
			layer->Draw(screen);
	}
	
	bool ObjectOnEvent(const AppEvent& event) override
	{            
		if (IsUIEvent(event, UIE_ON_INVALIDATE))
		{
			for (UI_Object* layer : layers)
				layer->Invalidate();
		}
        else if (IsUIEvent(event, UIE_ON_MOVE) || IsUIEvent(event, UIE_ON_RESIZE))
        {
			RefreshLayout();
        }
        else if (IsUIEvent(event, UIE_ON_SELECT))
        {
            UpdateLayer();
        }

		return layer_ptr && layer_ptr->OnEvent(event);
	}

	void UpdateLayer()
	{		
		for (UI_Object* layer : layers)
			layer->Select(false);
				
		if (IsSelected() && layers.get(layer_idx, layer_ptr)) 
		{
			layer_ptr->Select(true);
		}
		else
		{
			layer_ptr = nullptr;
		}
	}
	
	void OnPrev()
	{
		if (layer_idx > 0)
		{
			--layer_idx;
			UpdateLayer();
		}
	}
	
	void OnNext()
	{
		if (layer_idx + 1 < layers.size())
		{
			++layer_idx;
			UpdateLayer();
		}
	}
	
private:
	UI_ContainerType type;
	List<UI_Object*> layers;
	UI_ContainerAlign align{ UI_ContainerAlign::Begin };
	uint32_t margin{ 2 };
};

NEW_LETO_UI_END
