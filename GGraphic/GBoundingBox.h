#pragma once
namespace Common
{
	class GBoundingBox
	{
	public:
		using Point = glm::vec3;
		GBoundingBox(const Point& min, const Point& max);

		Point Min() const;
		Point Max() const;

	private:
		Point min_;
		Point min_;
	};
}

