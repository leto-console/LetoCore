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

#define NEW_LETO_UI_BEGIN namespace LetoNewUI {
#define NEW_LETO_UI_END	}
#define NEW_LETO_UI_USING using namespace LetoNewUI;

NEW_LETO_UI_BEGIN

enum UI_Events
{
	UIE_NONE = 0,

	UIE_ON_INVALIDATE,			///< UI object is invalidated
	UIE_ON_ENABLE,				///< UI object is enabled (by Enable), data: UID_TRUE | UID_FALSE
	UIE_ON_SELECT,				///< UI object is enabled (by Select), data: UID_TRUE | UID_FALSE
	UIE_ON_MOVE,				///< UI object is moved (by SetPosition)
	UIE_ON_RESIZE,				///< UI object is resized (by SetSize)
};

enum UI_DataFlag
{
	UID_ANY_DATA 	= 0,				///< Any UI data
	UID_NOT_SELF 	= (1u << 0),		///< 
	UID_FALSE 		= (1u << 1),		///< UI data `false`
	UID_TRUE 		= (1u << 2),		///< UI data `true`
};

class UI_Object
{
protected:
	enum
	{
		UIF_ENABLED,		///< Object enabled flag
		UIF_SELECTED, 		///< Object selected flag
	};

	void SendUIEvent(uint16_t event, uint32_t data = UID_ANY_DATA) 
	{
		OnEvent({ AE_UserInterface, event, data });
	}

	bool IsUIEvent(const AppEvent& check_event) const 
	{ 
		return check_event.source == AE_UserInterface; 
	}

	bool IsUIEvent(const AppEvent& check_event, uint16_t event_id, uint32_t event_data = UID_ANY_DATA) const
	{
		return IsUIEvent(check_event) && check_event.id == event_id && (check_event.data & event_data) == event_data;
	}

public:
	UI_Object(Point2_i size = {}, const LetoAllocator_V1* allocator = nullptr) 
		: size{ size }, childs{ allocator }
	{ }
	UI_Object(Point2_i size, UI_Object& parent)
		: UI_Object{ size, parent.GetAllocator() }
	{ }
	virtual ~UI_Object() = default;

	const LetoAllocator_V1* GetAllocator() const
	{
		return childs.get_allocator();
	}
		
	void Enable(bool enable) 
	{ 
		if (SetFlag(UIF_ENABLED, enable))
		{
			SendUIEvent(UIE_ON_ENABLE, enable ? UID_TRUE : UID_FALSE);
			Invalidate();
		}
	}
	bool IsEnabled() const { return CheckFlag(UIF_ENABLED); }
	
	void Select(bool select)
	{
		if (SetFlag(UIF_SELECTED, select))
		{
			SendUIEvent(UIE_ON_SELECT, select ? UID_TRUE : UID_FALSE);
			Invalidate();
		}
	}
	bool IsSelected() const { return CheckFlag(UIF_SELECTED); }

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
	UI_Object& AddChild(UI_Object* child, bool enable = false)
	{
		childs.push_back(child);
		Invalidate();
		if (enable) child->Enable(true);
		return *this;
	}
	
	/**
	 * @brief Добавить дочерний объект
	 */
	UI_Object& RemoveChild(UI_Object* child)
	{
		childs.remove(child);
		Invalidate();
		return *this;
	}
	
	/**
	 * @brief Обработка событий дочерними объектами
	 */
	bool OnEventChilds(const AppEvent& event);
		
	/**
	 * @return Флаг изменился
	 */
	bool SetFlag(uint8_t flag, bool state);
	
	bool CheckFlag(uint8_t flag) const;

private:
	uint32_t flags{};
	Point2_i pos{ }, size{ };

	void InvalidateRect();
};

NEW_LETO_UI_END
