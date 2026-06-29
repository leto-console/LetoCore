#include <DrawFunctions/DrawCircle.hpp>

#include "UI_Circle.hpp"

UI_Circle::UI_Circle(int radius, int outline)
	: radius { radius }, outline { outline }, color{ WhiteColor }, background{ BlackColor }
{
}

void UI_Circle::SetRadius(int radius)
{
	this->radius = radius;
}

int UI_Circle::GetRadius() const
{
	return radius;
}

void UI_Circle::SetOutline(int outline)
{
	this->outline = outline;
}

int UI_Circle::GetOutline() const
{
	return outline;
}

UI_Circle &UI_Circle::SetMainColor(RGBColor color)
{
	this->color = color; 
	return *this;
}

UI_Circle &UI_Circle::SetBackroundColor(RGBColor color)
{
	this->background = color; 
	return *this;
}

void UI_Circle::Draw(IScreen& screen, Point2_i offset)
{
	using namespace DrawFunctions;
		
	DrawCircle(screen, position + offset, radius, color);
	if (outline)
		DrawCircle(screen, position + offset, outline, background);
}

bool UI_Circle::ProcessInput(const AppEvent &event)
{
    return false;
}
