#include "Emoji.hpp"

#include "Bitmaps.h"

const uint32_t EMOJI_START_ID = 256;

#include <Data/StaticList.hpp>

StaticListView<BitmapData> GetEmojis()
{
	static StaticList<BitmapData, 16> BM_Emoji
	{
		BM_Emoji_1F47D,
		BM_Emoji_1F60E,
		BM_Emoji_1F914,
		BM_Emoji_1F920,
	};

	static bool isInitialized = false;

	if (!isInitialized)
	{
		for (int i = 0; i < BM_Emoji.Count(); ++i)
			BM_Emoji[i].ID = EMOJI_START_ID + i;

		isInitialized = true;
	}

	return BM_Emoji;
}
