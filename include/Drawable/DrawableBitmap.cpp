#include <Drawable/DrawableBitmap.hpp>
#include <DrawFunctions/DrawBitmap.hpp>

using namespace Drawable;

DrawableBitmap::DrawableBitmap(BitmapData& data)
	: data{ data }
{
}

void DrawableBitmap::Draw(IScreen& screen, Point2_i offset)
{
	if (!IsActive())
		return;
	DrawFunctions::DrawBitmap(screen, position + offset, data, WhiteColor);
}
