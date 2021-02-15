#pragma once
namespace Common
{
	namespace detial
	{
		template <class T>
		bool IsValid(T point)
		{
			return false;
		}

		bool IsValid(float p);

		bool IsValid(const glm::vec3& vec);

		bool IsValid(const std::vector<glm::vec3>& vec);
	}
}
