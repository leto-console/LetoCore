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
	typedef enum
	{
		UIE_FLAG_ENABLED,		///< Object enabled flag
		UIE_FLAG_SELECTED, 		///< Object selected flag
	};

	inline void SendUIEvent(uint16_t event, uint32_t data = 0) 
	{
		OnEvent({ AE_UserInterface, event, data });
	}

	inline bool IsUIEvent(const AppEvent& check_event, uint16_t ui_event, uint32_t data = 0) const
	{
		return check_event.source == AE_UserInterface && check_event.id == ui_event && check_event.data == data;
	}

public:
	UI_Object(Point2_i size, const LetoAllocator_V1* allocator = nullptr) 
		: size{ size }, childs{ allocator }
	{ }
	virtual ~UI_Object() = default;
		
	inline void Enable(bool enable) 
	{ 
		if (SetFlag(UIE_FLAG_ENABLED, enable))
		{
			SendUIEvent(UIE_ON_ENABLE, enable ? 1 : 0);
			Invalidate();
		}
	}
	inline bool IsEnabled() const { return CheckFlag(UIE_FLAG_ENABLED); }
	
	inline void Select(bool select)
	{
		if (SetFlag(UIE_FLAG_SELECTED, select))
		{
			SendUIEvent(UIE_ON_SELECT, select ? 1 : 0);
			Invalidate();
		}
	}
	inline bool IsSelected() const { return CheckFlag(UIE_FLAG_SELECTED); }

	inline void SetPosition(Point2_i pos) 
	{ 
		if (this->pos != pos)
		{
			SendUIEvent(UIE_ON_MOVE);
			Invalidate();
		}
		this->pos = pos;
	}
	inline Point2_i GetPosition() const { return pos; }
	
	inline void SetSize(Point2_i size) 
	{ 
		if (this->size != size)
		{
			SendUIEvent(UIE_ON_RESIZE);
			Invalidate();
		}
		this->size = size;
	}
	inline Point2_i GetSize() const { return size; }

	inline void Invalidate()
	{
		//leto::graphics::InvalidateRect(pos, size);
		for (UI_Object* child : childs)
			child->Invalidate();
		SendUIEvent(UIE_ON_INVALIDATE);
	}

	/**
	 * @brief Главная функция отрисовки
	 */
	void Draw(LetoScreen_V1* screen)
	{
		if (!IsEnabled()) return;
		/// Самый передний объект отрисовывается 
		/// самым последним
		for (UI_Object* child : childs)
			child->Draw(screen);
		ObjectDraw(screen);
	}
	
	/**
	 * @brief Главная функция фоновой обработки
	 */
	void Loop()
	{
		if (!IsEnabled()) return;
		ObjectLoop();
		for (UI_Object* child : childs)
			child->Loop();
	}
	
	/**
	 * @brief Главная функция обработки событий
	 */
	bool OnEvent(const AppEvent& event)
	{
		if (!IsEnabled()) return false;
		if (ObjectOnEvent(event)) return true;

		return OnEventChilds(event);
	}
	
protected:

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
		if (flag >= 8 * sizeof(flags)) return false;

		bool prev_state = CheckFlag(flag);
		if (prev_state == state) return false;
		
		if (state)
			flags |= (1 << flag);
		else
			flags &= ~(1 << flag);

		return true;
	}
	
	bool CheckFlag(uint8_t flag) const
	{
		if (flag >= 8 * sizeof(flags)) return false;
		
		return (flags >> flag) & 1;
	}

private:
	uint32_t flags{};
	Point2_i pos{ }, size{ };
	
	List<UI_Object*> childs;
};
