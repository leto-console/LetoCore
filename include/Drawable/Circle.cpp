#include <Drawable/Circle.hpp>
#include <DrawFunctions/DrawCircle.hpp>
#include "Circle.hpp"

using namespace Drawable;

Drawable::Circle::Circle(int radius, int outline)
	: radius { radius }, outline { outline }, color{ WhiteColor }, background{ BlackColor }
{
}

void Drawable::Circle::SetRadius(int radius)
{
	this->radius = radius;
}

int Drawable::Circle::GetRadius() const
{
	return radius;
}

void Drawable::Circle::SetOutline(int outline)
{
	this->outline = outline;
}

int Drawable::Circle::GetOutline() const
{
	return outline;
}

Circle &Drawable::Circle::SetMainColor(RGBColor color)
{
	this->color = color; 
	return *this;
}

Circle &Drawable::Circle::SetBackroundColor(RGBColor color)
{
	this->background = color; 
	return *this;
}

void Drawable::Circle::Draw(IScreen& screen, Point2_i offset)
{
	using namespace DrawFunctions;

	if (!IsActive())
		return;
		
	DrawCircle(screen, position + offset, radius, color);
	if (outline)
		DrawCircle(screen, position + offset, outline, background);
}
