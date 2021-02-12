#include "pch.h"
#include "GMathematic.h"
namespace Common
{
	namespace Mathematic
	{
		float DotProduct(const glm::vec3& v1, const glm::vec3& v2)
		{
			auto v3 = v1 * v2;
			float product = 0;
			for (int i = 0; i < 3; i++)
				product += v3[i];

			return product;
		}

		float ModuleOfVector(const glm::vec3& v)
		{
			return std::sqrt(abs(v[0] * v[0]) + abs(v[1] * v[1]) + abs(v[2] * v[2]));
		}

		float AngleBeetwnVector(const glm::vec3& v1, const glm::vec3& v2)
		{
			float dot = DotProduct(v1, v2);
			float _v1_ = ModuleOfVector(v1);
			float _v2_ = ModuleOfVector(v2);

			float cos_ = DotProduct(v1, v2) / (ModuleOfVector(v1) * ModuleOfVector(v2));
			return acos(cos_);
		}

		glm::vec3 MakeVector(const glm::vec3& start, const glm::vec3& end)
		{
			glm::vec3 v(end[0] - start[0], end[1] - start[1], end[2] - start[2]);
			return v;
		}

		float DistanseTo(const glm::vec3& v1, const glm::vec3& v2)
		{
				return std::sqrtf(
				std::abs(v2[0] - v1[0]) * std::abs(v2[0] - v1[0])  
				+
				std::abs(v2[1] - v1[1])* std::abs(v2[1] - v1[1])
				+
				std::abs(v2[2] - v1[2])* std::abs(v2[2] - v1[2])
				);
		}

		
	}
}