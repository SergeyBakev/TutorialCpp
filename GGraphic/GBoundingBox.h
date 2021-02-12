#pragma once
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

	private:
		Point min_;
		Point max_;
	};
}

