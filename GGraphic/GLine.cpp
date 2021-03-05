#include "pch.h"
#include "GLine.h"
#include "ge_utils.h"

namespace Common
{
    namespace Graphic
    {
        GLine::GLine(const GeometryPrimitive::Line& line) : line_(line)
        {

        }

        GLine::GLine(const glm::vec3& p1, const glm::vec3& p2) : line_(p1,p2)
        {

        }
        GBoundingBox GLine::OnGetBBox() const
        {
            return line_.GetBBox();
        }
        void GLine::OnDraw()
        {
            base::OnDraw();

            glm3Vectors vec = { line_.Start() , line_.End() };
            utils::draw_array(vec, GL_LINES);
        }
    }
}
