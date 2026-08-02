/**
 * @file Eyes.hpp
 * @date Apr 12, 2026
 * @author Rakhimov T.
 */

#ifndef INC_BITMAPS_EYES_HPP_
#define INC_BITMAPS_EYES_HPP_

#include "LibrariesExport.h"

#include <stdint.h>

#include <Graphics/BitmapData.hpp>
#include <Data/StaticListView.hpp>

/// Получить анимацию "глазок"
extern LETO_CORE_EXPORT StaticListView<BitmapData*> GetEyeAnim(bool left);

#endif
