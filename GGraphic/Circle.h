#pragma once
#include "GBoundingBox.h"
#include "GMathematic.h"

namespace Common
{
    namespace GeometryPrimitive
    {
        static const glm::vec3 xy_plane = glm::vec3(1, 1, 0);
        static const glm::vec3 yz_plane = glm::vec3(0, 1, 1);
        static const glm::vec3 zx_plane = glm::vec3(1, 0, 1);

        class Circle
        {
        public:
            Circle(const glm::vec3& center, const glm::vec3& plane, float radius);
            Circle(const glm::vec3& center, float radius);

            Circle( float center_x, float center_y, float center_z,
                    float x_plane, float y_plane, float z_plane,
                    float radius);
      
            Circle( float center_x, float center_y,
                    float x_plane, float y_plane,
                    float radius);

            inline float Radius() const { return radius_; }
            inline glm::vec3 Center() const { return center_; }
            inline glm::vec3 Plane() const { return plane_; }

            float Circumference() const;

        protected:
            Circle() = default;

        protected:
            float radius_;
            glm::vec3 plane_;
            glm::vec3 center_;

        };
    }
}