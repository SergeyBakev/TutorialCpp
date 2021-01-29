#pragma once
namespace Common
{
	namespace Mathematic
	{
		float DotProduct(const glm::vec3& v1, const glm::vec3& v2);

		float ModuleOfVector(const glm::vec3& v);

		float AngleBeetwnVector(const glm::vec3& v1, const glm::vec3& v);

		glm::vec3 MakeVector(const glm::vec3& start, const glm::vec3& end);
	}
}