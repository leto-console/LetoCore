/**
 * @brief ShiftScreen.hpp
 * @date Oct 17, 2026
 * @author Rakhimov T.
 */
#pragma once

#include <Graphics/IScreen.hpp>

class ShiftScreen : public IScreen
{
public:
    ShiftScreen(IScreen* screen, Point2_i shift) 
        : screen{ screen }, shift{ shift }
    { }

    int Width() const override { return screen->Width(); }
	int Height() const override { return screen->Height(); }

	void PixelSet(int x, int y, RGBColor color) override
    {
        screen->PixelSet(x + shift.x, y + shift.y, color);
    }
    
	RGBColor PixelGet(int x, int y) override
    {
        return screen->PixelGet(x + shift.x, y + shift.y);   
    }

	void FillScreen(RGBColor color) override { screen->FillScreen(color); }
	void ClearScreen() override { screen->ClearScreen(); }

protected:
    IScreen* screen;
    Point2_i shift;
};