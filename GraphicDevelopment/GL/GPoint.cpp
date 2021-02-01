#include "stdafx.h"
#include "GPoint.h"
namespace Common
{
	namespace Graphic
	{
		using namespace Common::Resources;

		GPoint::GPoint(double x, double y, ShaderProgramPtr& shader) : GraphicElementBase({ 1.0,0.0,0.0 },shader)
		{
			pnt_.x_ = x;
			pnt_.y_ = y;
		}

		GPoint::GPoint(const glm::vec3& pnt, ShaderProgramPtr& shader) : GraphicElementBase({ 1.0,0.0,0.0 },shader)
		{
			pnt_.x_ = pnt[0];
			pnt_.y_ = pnt[1];
		}

		GPoint::GPoint(const glm::vec4& pnt, ShaderProgramPtr& shader) : GraphicElementBase({ 1.0,0.0,0.0 },shader)
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
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(pnt_), &pnt_, GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
			glPointSize(10);
			glDrawArrays(GL_POINTS, 0, 1);
		}

	}
}
