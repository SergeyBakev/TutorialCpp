#pragma once
#include "GBoundingBox.h"
namespace Common
{
    namespace GeometryPrimitive
    {
        class Line
        {
        public:
            Line(const glm::vec3& start, const glm::vec3& end);

            glm::vec3 Start() const { return to_; }
            glm::vec3 End() const { return from_; }

            glm::vec3 Direction() const { return to_ - from_; }
            glm::vec3 PointAt(float t) const;


            GBoundingBox GetBBox() const;
        private:
            glm::vec3 from_;
            glm::vec3 to_;
        };
    }
}

