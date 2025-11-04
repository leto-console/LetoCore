/*
 * Emoji.hpp
 *
 *  Created on: Mar 01, 2026
 *      Author: Timur
 */

#ifndef INC_BITMAPS_EMOJI_HPP_
#define INC_BITMAPS_EMOJI_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

#include <Graphics/BitmapData.hpp>
#include <Data/StaticListView.hpp>

extern LIBRARIES_EXPORT StaticListView<BitmapData> GetEmojis();

#endif
