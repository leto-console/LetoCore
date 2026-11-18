/**
 * @file UI_Container.hpp
 * @date Nov 17, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <LetoAPI_V1/LetoAPI_V1.h>

#include <Input/CatchersNew/ButtonCatcher.hpp>
#include <Input/SystemInputID.hpp>

#include <UI/Base/UI_Alignable.hpp>
#include <UI/Base/UI_Object.hpp>

NEW_LETO_UI_BEGIN

enum class UI_ContainerType
{
	Vertical = 1,	///< Align to begin of layer
	Horizontal,		///< Align to center of layer
};

class UI_Container : public UI_Alignable
{
public:
	UI_Container(UI_ContainerType type, Point2_i size, const LetoAllocator_V1* allocator = nullptr)
        : UI_Alignable{ size, allocator }, type{ type }, layers{ allocator },
		prev_catcher{ this, &UI_Container::OnPrev },
		next_catcher{ this, &UI_Container::OnNext }
	{ 
		AddChild(&prev_catcher);
		AddChild(&next_catcher);
	}
	UI_Container(UI_ContainerType type, Point2_i size, UI_Object& parent)
        : UI_Container{ type, size, parent.GetAllocator() }
	{ }

	void InitBaseCatchers()
	{
		ButtonCatchPrev(type == UI_ContainerType::Horizontal ? SYSTEM_BTN_LEFT : SYSTEM_BTN_UP);
		ButtonCatchNext(type == UI_ContainerType::Horizontal ? SYSTEM_BTN_RIGHT : SYSTEM_BTN_DOWN);
	}
	
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
		RefreshAlign();
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

	enum class UI_ContainerAlign
	{
		Begin = 1,		///< Align to begin of layer
		Center,			///< Align to center of layer
		End				///< Align to end of layer
	};
	
	int& GetAxis(Point2_i& p, bool inverse = false) const
	{
		return (type == UI_ContainerType::Vertical) == !inverse ? p.y : p.x;
	}
	
	int GetAxis(const Point2_i& p, bool inverse = false) const
	{
		return (type == UI_ContainerType::Vertical) == !inverse ? p.y : p.x;
	}

	UI_ContainerAlign GetContainerAlign(bool inverse = false) const
	{
		UI_ContainerAlign align = UI_ContainerAlign::Begin;
		if ((type == UI_ContainerType::Vertical) == !inverse)
		{
			if (GetYAlign() == UI_YAlign::CENTER)
				align = UI_ContainerAlign::Center;
			else if (GetYAlign() == UI_YAlign::BOTTOM)
				align = UI_ContainerAlign::End;
		}
		else
		{
			if (GetXAlign() == UI_XAlign::CENTER)
				align = UI_ContainerAlign::Center;
			else if (GetXAlign() == UI_XAlign::RIGHT)
				align = UI_ContainerAlign::End;
		}
		return align;
	}

	bool RefreshAlign() override;
	void UpdateLayer();
	
	void ObjectDraw(LetoScreen_V1* screen) override
	{
		for (UI_Object* layer : layers)
			layer->Draw(screen);
	}
	
	bool ObjectOnEvent(const AppEvent& event) override;
	
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
	uint32_t margin{ 2 };
};

NEW_LETO_UI_END
