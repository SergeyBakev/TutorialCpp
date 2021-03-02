#include "pch.h"
#include "Circle.h"

Common::GeometryPrimitive::Circle::Circle(const glm::vec3& center, const glm::vec3& plane, float radius) :
    center_(center), plane_(plane), radius_(radius)
{

}

Common::GeometryPrimitive::Circle::Circle(const glm::vec3& center, float radius) : Circle(center, xy_plane, radius)
{
}

Common::GeometryPrimitive::Circle::Circle(	float center_x, float center_y, 
											float center_z, float x_plane, float y_plane, float z_plane, 
											float radius)
	: Circle({ center_x,center_y,center_z }, { x_plane,y_plane,z_plane }, radius)
{
}

Common::GeometryPrimitive::Circle::Circle(float center_x, float center_y, float x_plane, float y_plane, float radius) : Circle({ center_x,center_y,0 }, { x_plane,y_plane,0 }, radius)
{

}

float Common::GeometryPrimitive::Circle::Circumference() const
{
	return fabs(2 * glm::pi<float>() * radius_);
}
