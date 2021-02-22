#include "pch.h"
#include "GPoint.h"
#include "GVertexBufferObject.h"
namespace Common
{
	namespace Graphic
	{
		using namespace Common::Resources;

		GPoint::GPoint(double x, double y) 
		{
			pnt_.x_ = (float)x;
			pnt_.y_ = (float)y;
		}

		GPoint::GPoint(const glm::vec3& pnt) 
		{
			pnt_.x_ = pnt[0];
			pnt_.y_ = pnt[1];
		}

		GPoint::GPoint(const glm::vec4& pnt) 
		{
			pnt_.x_ = pnt.x;
			pnt_.y_ = pnt.y;
		}

		GBoundingBox GPoint::OnGetBBox() const
		{
			return GBoundingBox(glm::vec3(pnt_.x_, pnt_.y_, pnt_.z_));
		}

		void GPoint::OnDraw()
		{
			GraphicElementBase::OnDraw();
			DrawPoint();
		}

		void GPoint::DrawPoint()
		{
			GVertexBuffer vertex;
			vertex.Atach(&pnt_, sizeof(pnt_));
			vertex.Bind();

			glPointSize(GetSize());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
			
			glDrawArrays(GL_POINTS, 0, 1);
		}

		GPointPtr MakeGPoint(double x, double y)
		{
			return std::make_shared<GPoint>(x, y);
		}

		GPointPtr MakeGPoint(float x, float y)
		{
			return std::make_shared<GPoint>((double)x, (double)y);
		}

		GPointPtr MakeGPoint(double x, double y, double z)
		{
			return std::make_shared<GPoint>(x, y);
		}

		GPointPtr MakeGPoint(float x, float y, float z)
		{
			return std::make_shared<GPoint>((double)x, (double)y);
		}

		GPointPtr MakeGPoint(const glm::vec3& v)
		{
			return std::make_shared<GPoint>((double)v.x, (double)v.y);
		}

	}
}
