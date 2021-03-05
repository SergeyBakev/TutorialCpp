#pragma once
#include "Graphic.h"
#include "def_gl_object.h"
#include "Line.h"
namespace Common
{
    namespace Graphic
    {
        class GLine : public GraphicElementBase
        {
        public:
            DECLARE_G_OBJ(GLine, GraphicElementBase)

            GLine(const GeometryPrimitive::Line& line);
            GLine(const glm::vec3& p1, const glm::vec3& p2);

        protected:

            virtual GBoundingBox OnGetBBox() const override;
            virtual void OnDraw() override;

        private:
            GeometryPrimitive::Line line_;
        };

    }
}

