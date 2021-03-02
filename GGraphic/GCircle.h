#pragma once
#include "Graphic.h"
#include "def_gl_object.h"
#include "ShaderProgram.h"
#include "Circle.h"
#include "Arc.h"
namespace Common
{
    namespace Graphic
    {
        class GCircle : public GraphicElementBase
        {
        public:
            DECLARE_G_OBJ(GCircle, GraphicElementBase)

            GCircle(const Common::GeometryPrimitive::Circle& circle);

            GCircle(const Common::GeometryPrimitive::Circle& circle, const Resources::ShaderProgramPtr& circleShader);

            GCircle(const GeometryPrimitive::Arc& arc);

        protected:

            virtual GBoundingBox OnGetBBox() const override;
            virtual void OnDraw() override;

        private:
            Common::GeometryPrimitive::Arc arc_;
            Resources::ShaderProgramPtr selfShader_;
        };
    }
}

