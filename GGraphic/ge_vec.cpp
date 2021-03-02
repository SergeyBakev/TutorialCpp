#include "pch.h"
#include "ge_vec.h"

bool Common::is_paralel(const glm::vec3& v1, const glm::vec3& v2)
{
	return false;
}

bool Common::is_perpendicular(const glm::vec3& v1, const glm::vec3& v2)
{
	return glm::dot(v1, v2) == 0;
}

float Common::Length2(const glm::vec3& v)
{
	float len;
	float fx = fabs(v.x);
	float fy = fabs(v.y);
	float fz = fabs(v.z);

	if ((fx > (float)1e40))
		return 0.0;
	if ((fy > (float)1e40))
		return 0.0;
	if ((fz > (float)1e40))
		return 0.0;

	if (fy >= fx && fy >= fz)
	{
		len = fx; fx = fy; fy = len;
	}
	else if (fz >= fx && fz >= fy)
	{
		len = fx; fx = fz; fz = len;
	}

	if (fx > DBL_MIN)
	{
		len = 1.0f / fx;
		fy *= len;
		fz *= len;
		len = fx * sqrtf(1.0f + fy * fy + fz * fz);
	}
	else if (fx > 0.0 && _finite(fx))
		len = fx;
	else
		len = 0.0;

	return len;
}

bool Common::Unitize(glm::vec3& v)
{
	{
		// 15 September 2003 Dale Lear
		//     Added the DBL_MIN test.  See CVector3d::Length()
		//     for details.
		bool rc = false;
		float d = Length2(v);
		if (d > DBL_MIN)
		{
			d = 1.0f / d;
			v.x *= d;
			v.y *= d;
			v.z *= d;
			rc = true;
		}
		else if (d > 0.0 && _finitef(d))
		{
			_ASSERT(0);
			// This code is rarely used and can be slow.
			// It multiplies by 2^1023 in an attempt to 
			// normalize the coordinates.
			// If the renormalization works, then we're
			// ok.  If the renormalization fails, we
			// return false.
			glm::vec3 tmp;
			tmp.x = v.x * (float)8.9884656743115795386465259539451e+307;
			tmp.y = v.y * (float)8.9884656743115795386465259539451e+307;
			tmp.z = v.z * (float)8.9884656743115795386465259539451e+307;
			d = Length2(tmp);
			if (d > DBL_MIN)
			{
				d = 1.0f / d;
				v.x = tmp.x * d;
				v.y = tmp.y * d;
				v.z = tmp.z * d;
				rc = true;
			}
			else
			{
				v.x = 0.0;
				v.y = 0.0;
				v.z = 0.0;
			}
		}
		else
		{
			v.x = 0.0;
			v.y = 0.0;
			v.z = 0.0;
		}

		return rc;
	}

}
