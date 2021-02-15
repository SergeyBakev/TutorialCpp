#pragma once
using glm3Vectors = std::vector<glm::vec3>;

namespace Common
{
	class GBoundingBox
	{
	public:
		using Point = glm::vec3;
		GBoundingBox() = default;
		GBoundingBox(const Point& min, const Point& max);

		Point Min() const;
		Point Min(const Point& min);
		
		Point Max() const;
		Point Max(const Point& max);

		bool Set(const GBoundingBox& bbx);
		bool Set(const glm::vec3& point, int bGrowBox = false);

		bool GetCorners(glm3Vectors& box_corners) const;

		bool Union(const GBoundingBox& bbox);
		double MaximumDistanceTo(const glm::vec3& P) const;
	
		glm::vec3 Center() const;

		bool IsValid() const;

		glm::vec3 Diagonal() const; // max corner - min corner

	private:
		Point min_ = Point(1,0,0);
		Point max_ = Point(-1, 0, 0);
	};
}

