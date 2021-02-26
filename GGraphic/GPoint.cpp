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
			pnt_.x_ = pnt.x;
			pnt_.y_ = pnt.y;
			pnt_.z_ = pnt.z;
		}

		GPoint::GPoint(const glm::vec4& pnt) 
		{
			pnt_.x_ = pnt.x;
			pnt_.y_ = pnt.y;
			pnt_.z_ = pnt.z;
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
			glPointSize(GetSize());
			GVertexBuffer vertex;
			vertex.Atach(&pnt_, sizeof(pnt_));
			vertex.Bind();

			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
			
			glDrawArrays(GL_POINTS, 0, 1);
		}

		

		

	}
}
