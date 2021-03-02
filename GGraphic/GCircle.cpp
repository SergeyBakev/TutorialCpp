#include "pch.h"
#include "GCircle.h"
#include "ge_utils.h"

namespace Common
{
	namespace Graphic
	{
		GCircle::GCircle(const Common::GeometryPrimitive::Circle& circle) : arc_(circle)
		{
		}

		GCircle::GCircle(const Common::GeometryPrimitive::Circle& circle, const Resources::ShaderProgramPtr& circleShader) : arc_(circle), selfShader_(circleShader)
		{
		}

		GCircle::GCircle(const GeometryPrimitive::Arc& arc) : arc_(arc)
		{
		}

		GBoundingBox GCircle::OnGetBBox() const
		{
			GBoundingBox bbx(arc_.Center() - arc_.Radius(), arc_.Center() + arc_.Radius());
			return bbx;
		}
		void GCircle::OnDraw()
		{
			base::OnDraw();
			glEnable(GL_LINE_SMOOTH);
			if (selfShader_ == nullptr)
			{
				float count = 360.f;
				float angmin = arc_.AngleMin();
				float angmax = arc_.AngleMax();
				glm3Vectors points;
				for (int i = 0; i < count; i++)
				{
					float t = float(i / (count - 1.0));
					float tt = float((1.0f - t) * angmin + float(t * angmax));
					points.push_back(arc_.PointAt(tt));
				}

				Common::utils::draw_array(points, GL_LINE_STRIP);

			}
			else
			{

			}
			glDisable(GL_LINE_SMOOTH);
		}
	}
}
