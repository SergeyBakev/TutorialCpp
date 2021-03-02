#pragma once
#include "Circle.h"

namespace Common
{
    namespace GeometryPrimitive
    {
        class Arc : public Circle
        {
        public:

            Arc(const Circle& circle);
            Arc(const glm::vec3& center, const glm::vec3& plane, float radius, float angMin, float angMax);
            Arc(const glm::vec3& center, float radius, float angMin, float angMax);

            glm::vec3 PointAt(float p) const;
            glm::vec3 Begin() const;
            glm::vec3 Mid() const;
            glm::vec3 End() const;

            float AngleMin() const { return angleMin_; };
            float AngleMax() const { return angleMax_; };
        private:
            float angleMin_;
            float angleMax_;
        };
    }
}

