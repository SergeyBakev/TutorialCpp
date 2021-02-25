#pragma once
#include "ge_color.h"

namespace Common
{
	namespace utils
	{
		bool draw_array(const glm3Vectors& points, GLenum drawMode);
		bool draw_array(const glm3Vectors& points, GLenum drawMode, const ColorRGB& color);

		bool draw_array_object(const glm3Vectors& points, GLenum drawMode);
		bool draw_array_object(const glm3Vectors& points, GLenum drawMode, const ColorRGB& color);
		
		bool draw_line(const glm::vec3& p0, const glm::vec3& p1);
		bool draw_line(const glm::vec3& p0, const glm::vec3& p1, const ColorRGB& color);

		glm::vec4 to_vec4(const glm::vec3& v);
		glm::vec3 to_vec3(const glm::vec4& v);
	}
}