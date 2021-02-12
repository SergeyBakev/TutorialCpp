#include "pch.h"
#include "GPoint.h"
#include "GVertexBufferObject.h"
namespace Common
{
	namespace Graphic
	{
		using namespace Common::Resources;

		GPoint::GPoint(double x, double y) : GraphicElementBase({ 1.0,0.0,0.0 })
		{
			pnt_.x_ = x;
			pnt_.y_ = y;
		}

		GPoint::GPoint(const glm::vec3& pnt) : GraphicElementBase({ 1.0,0.0,0.0 })
		{
			pnt_.x_ = pnt[0];
			pnt_.y_ = pnt[1];
		}

		GPoint::GPoint(const glm::vec4& pnt) : GraphicElementBase({ 1.0,0.0,0.0 })
		{
			pnt_.x_ = pnt.x;
			pnt_.y_ = pnt.y;
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

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
			glPointSize(10);
			glDrawArrays(GL_POINTS, 0, 1);
		}

	}
}
