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
	IFont* font{};
	MenuStyle style = MenuStyle::STYLE_1;
	MenuHorizonAlignment horizon_align = MenuHorizonAlignment::LEFT;
	MenuVerticalAlignment vertical_align = MenuVerticalAlignment::TOP;

	int visible_elements{};				/// Количество видимых элементов меню (по умолчанию 8)

	int currentID{};					/// Индекс выделенной строки
	int topID{};						/// Индекс первой строки (сверху)

	bool reset_on_show{ true };			/// Сбрасывать ли автоматически меню при отображении
	bool ready = false;					/// В меню выбран элемент

    StaticListView<StaticText32> texts;

	InputCatcher<MenuHolder> up_catcher, down_catcher;

public:
	MenuHolder(StaticListView<StaticText32> texts, uint8_t visible_elements = 8, Point2_i position = {});

	void InitBaseCatchers();
	
	void RegUpEvent(IsEventFunc is_event);
	void RegDownEvent(IsEventFunc is_event);

	void OnShow() override;

	uint8_t Count() const;

	uint8_t GetItemsCount() const { return texts.Count(); }
	uint8_t GetCurrentID() const { return currentID; }
	StaticText32 GetCurrentText() const { return texts[currentID]; }

	// Установить горизонтальное выравнивание
	void SetHorizonAlignment(MenuHorizonAlignment align);

	// Установить вертикальное выравнивание
	void SetVerticalAlignment(MenuVerticalAlignment align);

	void SetStyle(MenuStyle style, IFont* font = nullptr);

	/// @brief Сбрасывать ли автоматически меню при отображении
	/// @param reset при `true` позиция курсора в `OnShow` в меню будет автоматически сброшена
	void SetResetOnShow(bool reset);

	StaticText32 GetText(uint8_t ID) const;

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
