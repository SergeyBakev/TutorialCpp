#include "pch.h"
#include "ge_validation.h"

namespace Common
{
	namespace detial
	{
		bool IsValid(float p)
		{
			return !std::isnan(p) && !std::isinf(p);
		}

		bool IsValid(const glm::vec3& v)
		{
			return IsValid(v.x) && IsValid(v.y) && IsValid(v.z);
		}


		bool IsValid(const std::vector<glm::vec3>& vec)
		{
			for (const auto& v : vec)
				if (!IsValid(v))
					return false;
			
			return true;
		}
	}
}
