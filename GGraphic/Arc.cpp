#include "pch.h"
#include "Arc.h"

Common::GeometryPrimitive::Arc::Arc(const Circle& circle)
{
    radius_ = circle.Radius();
    plane_ = circle.Plane();
    center_ = circle.Center();
    angleMin_ = 0;
    angleMax_ = 2 * glm::pi<float>();
}

Common::GeometryPrimitive::Arc::Arc(const glm::vec3& center, const glm::vec3& plane, float radius, float angMin, float angMax) : Circle(center, plane, radius)
{
    angleMin_ = angMin;
    angleMax_ = angMax;
}

Common::GeometryPrimitive::Arc::Arc(const glm::vec3& center, float radius, float angMin, float angMax) : Circle(center, xy_plane, radius)
{
    angleMin_ = angMin;
    angleMax_ = angMax;
}

glm::vec3 Common::GeometryPrimitive::Arc::PointAt(float p) const
{
    return center_ + Common::Mathematic::RotateZ(p, radius_);
}

glm::vec3 Common::GeometryPrimitive::Arc::Begin() const
{
    return PointAt(angleMax_);
}

glm::vec3 Common::GeometryPrimitive::Arc::Mid() const
{
    return PointAt(0.5f * (angleMin_ + angleMax_));
}

glm::vec3 Common::GeometryPrimitive::Arc::End() const
{
    return PointAt(angleMin_);
}
