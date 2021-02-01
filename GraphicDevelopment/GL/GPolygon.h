#pragma once
#include "Graphic.h"
#include "..\Resources\ShaderProgram.h"
namespace Common
{
	namespace Graphic
	{
        class GPolygon : public GraphicElementBase
        {
        public:
            GPolygon(Resources::ShaderProgramPtr& shader) : GraphicElementBase({ 0.0,0.0,0.0 },shader)
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