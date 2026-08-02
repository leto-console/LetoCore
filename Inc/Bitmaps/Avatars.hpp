/*
 * Bitmaps.hpp
 *
 *  Created on: Feb 27, 2026
 *      Author: Timur
 */

#ifndef INC_AUTH_AVATARS_HPP_
#define INC_AUTH_AVATARS_HPP_

#include <stdint.h>

#include "Bitmaps.h"
#include "LibrariesExport.h"

#include <Data/StaticListView.hpp>

extern LETO_CORE_EXPORT bool GetAvatarByID(uint32_t ID, BitmapData& data);

extern LETO_CORE_EXPORT StaticListView<BitmapData> GetAvatars();

#endif
