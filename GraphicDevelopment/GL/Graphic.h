#pragma once
#include "..\Resources\ShaderProgram.h"
namespace Common
{
	namespace Graphic
	{
        struct RGBColor
        {
            float red;
            float green;
            float blue;
        };

        class GraphicElement
        {
        public:

            virtual ~GraphicElement()
            {

            }
            virtual void Scale(float x, float y, float z) = 0;
            virtual void Translate(float x, float y, float z) = 0;
            virtual void Rotate(float angle,float x, float y, float z) = 0;

            virtual void MultMatrix(glm::mat4 transform) = 0;
            virtual glm::mat4 GetTransofrm() const = 0;

            virtual void Draw() = 0;
        };

        using GraphicElementPtr = std::shared_ptr<GraphicElement>;

        class GraphicElementBase : public GraphicElement
        {
        public:

            virtual void Draw() override;
            virtual ~GraphicElementBase();
            virtual void Scale(float x, float y, float z) override;
            virtual void Translate(float x, float y, float z) override;
            virtual void Rotate(float angle, float x, float y, float z) override;

            virtual glm::mat4 GetTransofrm() const override;
            virtual void MultMatrix(glm::mat4 transform) override;

        protected:
            GraphicElementBase(RGBColor color, Resources::ShaderProgramPtr& shader) : color_(color), shader_(shader) {}

            virtual void OnDraw() {};
            virtual void OnScale(float x, float y, float z);
            virtual void OnTranslate(float x, float y, float z) {};
            virtual void OnRotate(float angle, float x, float y, float z);


        protected:

            Resources::ShaderProgramPtr shader_;

        private:
            RGBColor color_;
            glm::mat4 model_ = glm::identity<glm::mat4>();
            
        };
	}
}