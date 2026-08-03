/*
 * LambdaAnimation.hpp
 *
 *  Created on: Mar 15, 2026
 *      Author: Timur
 */

#ifndef INC_UI_ANIMATION_LAMBDA_ANIMATION_HPP_
#define INC_UI_ANIMATION_LAMBDA_ANIMATION_HPP_

#include "LibrariesExport.h"

#include <UI/Animation/Animation.hpp>

#include <Graphics/BitmapData.hpp>
#include <Data/StaticListView.hpp>

// Анимация с проигрывания кадров
class LETO_CORE_EXPORT CartoonAnimation : public Animation
{
protected:
	StaticListView<BitmapData*> bitmaps;
	const BitmapData* current{ nullptr };
	bool repeat = false;

public:
	CartoonAnimation(StaticListView<BitmapData*> bitmaps, int ms_on_cadr, bool repeat = false);

	const BitmapData* GetCurrent() const;

	void Tick() override;
};

#endif