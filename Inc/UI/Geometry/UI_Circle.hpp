/**
 * @file UI_Circle.hpp
 * @date Jun 29, 2026
 * @author Rakhimov T.
 */

#ifndef INC_UI_GEOMETRY_UI_CIRCLE_HPP_
#define INC_UI_GEOMETRY_UI_CIRCLE_HPP_

#include "LibrariesExport.h"

#include <SceneManager/ISceneObject.hpp>

class LETO_CORE_EXPORT UI_Circle : public ISceneObject
{
protected:
	int radius{}, outline{};

	RGBColor color, background;

public:
	// Радиус и размер контура (при outline = 0 круг заливается)
	explicit UI_Circle(int radius = 0, int outline = 0);

	void SetRadius(int radius);
	int GetRadius() const;

	void SetOutline(int outline);
	int GetOutline() const;

	UI_Circle& SetMainColor(RGBColor color);
	UI_Circle& SetBackroundColor(RGBColor color);

	void Draw(IScreen& screen, Point2_i offset = {}) override;
	bool ProcessInput(const AppEvent& event) override;
};

#endif