#include <UI/Animation/CartoonAnimation.hpp>

CartoonAnimation::CartoonAnimation(StaticListView<BitmapData*> bitmaps, int ms_on_cadr, bool repeat)
	: Animation{ ms_on_cadr * (int) bitmaps.Count() }, bitmaps{ bitmaps }, repeat{ repeat }
{
}

const BitmapData* CartoonAnimation::GetCurrent() const
{ 
	return current; 
}

void CartoonAnimation::Tick()
{
	if (repeat && GetProgress() >= 1.0f)
		Play();
	if (GetProgress() == 0.0f && !repeat)
	{
		current = nullptr;
		return;
	}
	int cadr = static_cast<int>(GetProgress() * bitmaps.Count());
	if (cadr < bitmaps.Count() && cadr >= 0)
		current = bitmaps[cadr];
}
