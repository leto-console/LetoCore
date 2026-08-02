#include "Avatars.hpp"

#include "Emoji.hpp"

const BitmapData* GetAvatarByID(uint32_t ID)
{
	for (const BitmapData& bitmap : GetAvatars())
	{
		if (bitmap.ID == ID)
		{
			return &bitmap;
		}
	}
	return nullptr;
}

#include <Data/StaticList.hpp>

StaticListView<BitmapData> GetAvatars()
{
	static StaticList<BitmapData, 16> BM_Avas
	{
		BM_Ava0,
		BM_Ava1,
		BM_Ava2,
	};

	static bool isInitialized = false;

	if (!isInitialized)
	{
		for (int i = 0; i < BM_Avas.Count(); ++i)
			BM_Avas[i].ID = i;

		for (const BitmapData& emoji : GetEmojis())
			BM_Avas.Push(emoji);

		isInitialized = true;
	}

	return BM_Avas;
}
