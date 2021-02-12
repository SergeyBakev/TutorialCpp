#pragma once
#include "Graphic.h"
#include "ShaderProgram.h"
namespace Common
{
	namespace Graphic
	{
        class GPolygon : public GraphicElementBase
        {
        public:
            GPolygon() : GraphicElementBase({ 0.0,0.0,0.0 })
            {

            }

            void AddPoint(float x, float y, float z = 1);

        protected:
            virtual void OnDraw() override;

        private:
            std::vector<float> vertexes_;
        };
	}
}