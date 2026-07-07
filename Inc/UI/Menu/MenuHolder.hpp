/**
 * @file MenuHolder.hpp
 * @date Mar 28, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_MENU_MENU_HOLDER_HPP_
#define INC_UI_MENU_MENU_HOLDER_HPP_

#include "LibrariesExport.h"

#include <Graphics/IFont.hpp>
#include <Drawable/IDrawable.hpp>
#include <SceneManager/ISceneObject.hpp>
#include <Data/StaticText.hpp>
#include <Data/StaticListView.hpp>
#include <Input/InputCatcher.hpp>

enum class MenuHorizonAlignment : uint8_t
{
	LEFT,		// Выравнивание по левому краю
	CENTER,		// Выравнивание по центру
};

enum class MenuVerticalAlignment : uint8_t
{
	TOP,		// > наверху
	CENTER,		// > посередине
};

enum class MenuStyle : uint8_t
{
	STYLE_1,	// Стиль "> Элемент"
	STYLE_2,	// Стиль "> Элемент" с обводкой текста
	STYLE_3,	// Стиль "Элемент" с обводкой текста
};

class LIBRARIES_EXPORT MenuHolder : public ISceneObject
{
protected:
	int CharWidth = 8, CharHeight = 8;
	const IFont* font{};
	MenuStyle style = MenuStyle::STYLE_1;
	MenuHorizonAlignment horizon_align = MenuHorizonAlignment::LEFT;
	MenuVerticalAlignment vertical_align = MenuVerticalAlignment::TOP;

	int visible_elements{};				/// Количество видимых элементов меню (по умолчанию 8)

	int currentID{};					/// Индекс выделенной строки
	int topID{};						/// Индекс первой строки (сверху)

	bool reset_on_show{ true };			/// Сбрасывать ли автоматически меню при отображении
	bool ready_logic = false;			/// Включение логики `IsResultReady`
	bool ready = false;					/// В меню выбран элемент

	InputCatcher<MenuHolder> up_catcher, down_catcher, enter_cather;

public:
	MenuHolder(uint8_t visible_elements = 8, Point2_i position = {}, bool ready_logic = false);

	void InitBaseCatchers();
	void EnableReadyLogic();	/// TODO: Сделать такую логику по умолчанию, пока что сохранено для обратной совместимости
	
	void RegUpEvent(IsEventFunc is_event);
	void RegDownEvent(IsEventFunc is_event);
	void RegEnterEvent(IsEventFunc is_event);

	void OnShow() override;

	virtual uint8_t Count() const = 0;
	virtual const StaticText32& GetText(uint8_t idx) const = 0;

	uint8_t GetCurrentID() const { return currentID; }
	StaticText32 GetCurrentText() const { return GetText(currentID); }

	// Установить горизонтальное выравнивание
	void SetHorizonAlignment(MenuHorizonAlignment align);

	// Установить вертикальное выравнивание
	void SetVerticalAlignment(MenuVerticalAlignment align);

	void SetStyle(MenuStyle style, const IFont* font = nullptr);

	/// @brief Сбрасывать ли автоматически меню при отображении
	/// @param reset при `true` позиция курсора в `OnShow` в меню будет автоматически сброшена
	void SetResetOnShow(bool reset);

	void ResetCurrentID();

	bool IsResultReady(int& idx) const;
	void SubmitReady();

	void Up();
	void Down();
	void Enter();

	void Draw(IScreen& screen, Point2_i offset = {}) override;
	bool ProcessInput(const AppEvent& event) override;
};

#endif
