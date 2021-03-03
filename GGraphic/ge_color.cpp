#include "pch.h"
#include "ge_color.h"

namespace Common
{
	ColorRGB& ColorRGB::operator=(const glm::vec3& color)
	{
		r = color.x;
		g = color.y;
		b = color.z;

		return *this;
	}
	
	
}