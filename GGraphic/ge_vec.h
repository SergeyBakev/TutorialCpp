#pragma once
namespace Common
{
	bool is_paralel(const glm::vec3& v1, const glm::vec3& v2);
	bool is_perpendicular(const glm::vec3& v1, const glm::vec3& v2);
	float Length2(const glm::vec3& v);
	bool Unitize(glm::vec3& v);
}