#include "Eyes.hpp"

#include "BM_Eyes.h"

const uint32_t EMOJI_START_ID = 256;

#include <Data/StaticList.hpp>

StaticListView<BitmapData*> GetEyeAnim(bool left)
{
    static StaticList<BitmapData*, 32> eye_left
    {
        &BM_Eye5,
        &BM_Eye5,
        &BM_Eye5,
        &BM_Eye4,
        &BM_Eye3,
        &BM_Eye2,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye3,
        &BM_Eye5,
        &BM_Eye3,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
    };

    static StaticList<BitmapData*, 32> eye_right
    {
        &BM_Eye5,
        &BM_Eye5,
        &BM_Eye5,
        &BM_Eye4,
        &BM_Eye3,
        &BM_Eye2,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
        &BM_Eye1,
    };

	static bool isInitialized = false;

	if (!isInitialized)
	{
		//for (int i = 0; i < BM_Emoji.Count(); ++i)
		//	BM_Emoji[i].ID = EMOJI_START_ID + i;

		isInitialized = true;
	}

	return left ? eye_left : eye_right;
}
