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

        protected:

            virtual GBoundingBox OnGetBBox() const override;
            virtual void OnDraw() override;

        private:
            GeometryPrimitive::Line line_;
        };

    }
}

