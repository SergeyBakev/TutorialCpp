#pragma once
#include "Graphic.h"
#include "ResourceManager.h"
#include "def_gl_object.h"
namespace Common
{
	namespace Graphic
	{
        struct Point2D
        {
            float x_;
            float y_;
            float z_ = 0;
        };

        class GPoint : public GraphicElementBase
        {
        public:
            DECLARE_G_OBJ(GPoint, GraphicElementBase)
            GPoint(double x, double y);
            GPoint(const glm::vec3& pnt);
            GPoint(const glm::vec4& pnt);

            Point2D GetCord() const { return pnt_; }

        protected:

            virtual GBoundingBox OnGetBBox() const override;
            virtual void OnDraw() override;


        private:
            void DrawPoint();

        private:
            Point2D pnt_;

            // Inherited via GraphicElementBase
        
        };
	}
}
