/*
 * Graph.hpp
 *
 *  Created on: Oct 21, 2025
 *      Author: Timur
 */

#ifndef INC_DRAWABLE_MANAGER_GRAPH_HPP_
#define INC_DRAWABLE_MANAGER_GRAPH_HPP_

#include <stdint.h>
#include <string.h>

#include <Drawable/IDrawable.hpp>
#include <Graphics/IScreen.hpp>
#include <DrawFunctions/DrawLine.hpp>

#include <Data/SingleData.hpp>
#include <Data/SourceData.hpp>

 // Графическое изображение графика
template <typename T, int Width>
class Graph : public IDrawable
{
protected:
	int counter = 0;
	int height, scale{ 1 }, ptr{ 0 };
	T data[Width]{ };
	T amp{};
public:
	Graph(int height, T amp)
		: height{height}, amp{amp}
	{ 
	}

	virtual void AddValue(SingleData<T> data)
	{
		T val; data.Get(val);
		AddValue(val);
	}
	virtual void AddValue(SourceData<T> data)
	{
		T val; data.Get(val);
		AddValue(val);
	}
	virtual void AddValue(T val)
	{
		if (counter++ % scale != 0) return;
		memcpy(data, data + 1, (Width - 1) * sizeof(T));
		data[Width - 1] = val;
		if (ptr < Width) ptr++;
	}
	virtual void Clear() { ptr = 0; }
	virtual void SetScale(int scale) { Clear(); this->scale = scale; }
	virtual int GetScale() { return scale; }

	void Draw(IScreen& screen, Point2_i offset = {}) override
	{
		using namespace DrawFunctions;

		// Ось OX
		DrawLine(screen, offset + Point2_i{0, height / 2}, offset + Point2_i{Width / 4, height / 2}, WhiteColor);
		// Ось OY
		DrawLine(screen, offset, offset + Point2_i{0, height}, WhiteColor);

		for (int x = Width - ptr; x < Width; ++x)
		{
			T val = data[x];
			int y = 0;
			if (val > amp) y = height;
			else if (val < -amp) y = 0;
			else y = static_cast<int>(height * (amp + val) / 2);
			screen.PixelSet(offset + Point2_i{x, y}, WhiteColor);
		}
	}
};

#endif
