#pragma once
#include "ShaderProgram.h"
#include "GBoundingBox.h"

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
            virtual GBoundingBox GetBBox() const = 0;
            virtual void SetActiveShader(const Common::Resources::ShaderProgramPtr& shader) = 0;


            virtual void SetSize(float size) = 0;
            virtual float GetSize() const = 0;

            virtual Common::Resources::ShaderProgramPtr GetActiveShader() const = 0;

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

            virtual void MultMatrix(glm::mat4 transform) override;

            virtual glm::mat4 GetTransofrm() const override;
            virtual GBoundingBox GetBBox() const override;

            virtual void SetActiveShader(const Common::Resources::ShaderProgramPtr& shader) override;
            virtual Resources::ShaderProgramPtr GetActiveShader() const  override;

            void SetSize(float size) override;
            float GetSize() const override;

            void SetColor(float r, float g, float b);

        protected:
            GraphicElementBase() {}

            virtual void OnDraw() {};
            virtual GBoundingBox OnGetBBox() const { return {}; };
            virtual void OnScale(float x, float y, float z);
            virtual void OnTranslate(float x, float y, float z);
            virtual void OnRotate(float angle, float x, float y, float z);

        private:
            Resources::ShaderProgramPtr shader_;
            RGBColor color_ = { 0,0,0 };
            glm::mat4 model_ = glm::identity<glm::mat4>();
            float size_ = 1.f;
        };
	}
}