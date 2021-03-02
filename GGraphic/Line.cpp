#include "pch.h"
#include "Line.h"

namespace Common
{
    namespace GeometryPrimitive
    {
        Common::GeometryPrimitive::Line::Line(const glm::vec3& start, const glm::vec3& end) : to_(end), from_(start) {}
        glm::vec3 Line::PointAt(float t) const
        {
            float s = 1 - t;
            return
            {
                (from_.x == to_.x) ? from_.x : s * from_.x + t * to_.x,
                 (from_.x == to_.x) ? from_.x : s * from_.y + t * to_.y,
                 (from_.x == to_.x) ? from_.x : s * from_.z + t * to_.z
            };
        }

        GBoundingBox Line::GetBBox() const
        {
            GBoundingBox bbox;
            GBoundingBox::GetPointListBoundingBox(3, 0, 2, 3, glm::value_ptr(from_), bbox, 0, nullptr);
            return bbox;
        }
    }
}

