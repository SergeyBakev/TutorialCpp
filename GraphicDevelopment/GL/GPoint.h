#pragma once
#include "Graphic.h"
#include "..\Resources\ResourceManager.h"
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

            GPoint(double x, double y);
            GPoint(const glm::vec3& pnt);
            GPoint(const glm::vec4& pnt);

            Point2D GetCord() const { return pnt_; }

        protected:
            virtual void OnDraw() override;


        private:
            void DrawPoint();

        private:
            Point2D pnt_;
        };

        using GPoint2DPtr = std::shared_ptr<GPoint>;
	}
}
