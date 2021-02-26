#pragma once

namespace Common
{
	struct ColorRGB
	{
		float r;
		float g;
		float b;
	};

	static ColorRGB RED = { 1,0,0 };
	static ColorRGB GREEN = { 0,1,0 };
	static ColorRGB BLUE = { 0,0,1 };
	static ColorRGB BLACK = { 0,0,0 };
}