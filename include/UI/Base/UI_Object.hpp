/**
 * @file UI_Object.hpp
 * @date Nov 15, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <cstdint>

#include <Data/Point2.hpp>
#include <Data/List.hpp>

#include <LetoABI/AppEventSources.h>
#include <LetoABI/AppEvent.h>
#include <LetoAPI_V1/LetoAPI_V1.h>

enum UI_Events
{
	UIE_NONE = 0,

	UIE_ON_INVALIDATE,			///< UI object is invalidated
	UIE_ON_ENABLE,				///< UI object is enabled (by Enable), data = 0 - false, 1 - true
	UIE_ON_SELECT,				///< UI object is enabled (by Select), data = 0 - false, 1 - true
	UIE_ON_MOVE,				///< UI object is moved (by SetPosition)
	UIE_ON_RESIZE,				///< UI object is resized (by SetSize)
};

class UI_Object
{
protected:
	enum
	{
		UIE_FLAG_ENABLED,		///< Object enabled flag
		UIE_FLAG_SELECTED, 		///< Object selected flag
	};

	void SendUIEvent(uint16_t event, uint32_t data = -1u) 
	{
		OnEvent({ AE_UserInterface, event, data });
	}

	bool IsUIEvent(const AppEvent& check_event, uint16_t ui_event, uint32_t data = -1u) const
	{
		return check_event.source == AE_UserInterface && check_event.id == ui_event && (data == -1u || check_event.data == data);
	}

public:
	UI_Object(Point2_i size, const LetoAllocator_V1* allocator = nullptr) 
		: size{ size }, childs{ allocator }
	{ }
	virtual ~UI_Object() = default;
		
	void Enable(bool enable) 
	{ 
		if (SetFlag(UIE_FLAG_ENABLED, enable))
		{
			SendUIEvent(UIE_ON_ENABLE, enable ? 1 : 0);
			Invalidate();
		}
	}
	bool IsEnabled() const { return CheckFlag(UIE_FLAG_ENABLED); }
	
	void Select(bool select)
	{
		if (SetFlag(UIE_FLAG_SELECTED, select))
		{
			SendUIEvent(UIE_ON_SELECT, select ? 1 : 0);
			Invalidate();
		}
	}
	bool IsSelected() const { return CheckFlag(UIE_FLAG_SELECTED); }

	void SetPosition(Point2_i pos) 
	{ 
		if (this->pos != pos)
		{
			this->pos = pos;
			SendUIEvent(UIE_ON_MOVE);
			Invalidate();
		}
	}
	Point2_i GetPosition() const { return pos; }
	
	void SetSize(Point2_i size) 
	{ 
		if (this->size != size)
		{
			this->size = size;
			SendUIEvent(UIE_ON_RESIZE);
			Invalidate();
		}
	}
	Point2_i GetSize() const { return size; }

	void Invalidate();

	/**
	 * @brief Главная функция отрисовки
	 */
	void Draw(LetoScreen_V1* screen);
	
	/**
	 * @brief Главная функция фоновой обработки
	 */
	void Loop();
	
	/**
	 * @brief Главная функция обработки событий
	 */
	bool OnEvent(const AppEvent& event);
	
protected:
	List<UI_Object*> childs;

	virtual void ObjectDraw(LetoScreen_V1* screen) { (void)screen; };
	virtual void ObjectLoop() { };
	virtual bool ObjectOnEvent(const AppEvent& event) { (void)event; return false; };

	/**
	 * @brief Добавить дочерний объект
	 * @note Добавленный объект будет отрисован на самом переднем плане
	 */
	UI_Object& AddChild(UI_Object* child)
	{
		childs.push_back(child);
		return *this;
	}
	
	/**
	 * @brief Добавить дочерний объект
	 */
	UI_Object& RemoveChild(UI_Object* child)
	{
		childs.remove(child);
		return *this;
	}
	
	/**
	 * @brief Обработка событий дочерними объектами
	 */
	bool OnEventChilds(const AppEvent& event)
	{
		for (UI_Object* child : childs)
		{
			if (child->OnEvent(event)) 
				return true;
		}
		return false;
	}
	
	/**
	 * @return Флаг изменился
	 */
	bool SetFlag(uint8_t flag, bool state) 
	{
		if (flag >= 8u * sizeof(flags)) return false;

		bool prev_state = CheckFlag(flag);
		if (prev_state == state) return false;
		
		if (state)
			flags |= (1u << flag);
		else
			flags &= ~(1u << flag);

		return true;
	}
	
	bool CheckFlag(uint8_t flag) const
	{
		if (flag >= 8u * sizeof(flags)) return false;
		
		return (flags >> flag) & 1u;
	}

private:
	uint32_t flags{};
	Point2_i pos{ }, size{ };
};
