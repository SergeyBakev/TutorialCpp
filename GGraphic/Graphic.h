#pragma once
#include "ShaderProgram.h"
#include "GBoundingBox.h"
#include "ge_color.h"

namespace Common
{
	namespace Graphic
	{
        using GraphicElementPtr = std::shared_ptr<class GraphicElement>;

        class GraphicElement 
        {
        public:

            virtual ~GraphicElement() = default;
            virtual GraphicElement& Scale(float x, float y, float z) = 0;
            virtual GraphicElement& Scale(float scaleFactor) = 0;
            virtual GraphicElement& Translate(float x, float y, float z) = 0;
            virtual GraphicElement& Rotate(float angle,float x, float y, float z) = 0;
            virtual GraphicElement& MultMatrix(glm::mat4 transform) = 0;
            virtual glm::mat4 GetTransofrm() const = 0;
            virtual GBoundingBox GetBBox() const = 0;
            virtual void SetActiveShader(const Common::Resources::ShaderProgramPtr& shader) = 0;


            virtual GraphicElement& SetSize(float size) = 0;

            virtual float GetSize() const = 0;

            virtual GraphicElement& SetColor(float r, float g, float b) =0;
            virtual GraphicElement& SetColor(const ColorRGB& color) =0;
            virtual ColorRGB GetColor() const = 0;

            virtual Common::Resources::ShaderProgramPtr GetActiveShader() const = 0;

            virtual void Draw() = 0;
        };

       

        class GraphicElementBase : public GraphicElement /*, public std::enable_shared_from_this<GraphicElementBase>*/
        {
        public:

            virtual void Draw() override;
            virtual ~GraphicElementBase();
            virtual GraphicElement& Scale(float x, float y, float z) override;
            virtual GraphicElement& Scale(float scaleFactor)override;

            virtual GraphicElement& Translate(float x, float y, float z) override;
            virtual GraphicElement& Rotate(float angle, float x, float y, float z) override;

            virtual GraphicElement& MultMatrix(glm::mat4 transform) override;

            virtual glm::mat4 GetTransofrm() const override;
            virtual GBoundingBox GetBBox() const override;

            virtual void SetActiveShader(const Common::Resources::ShaderProgramPtr& shader) override;
            virtual Resources::ShaderProgramPtr GetActiveShader() const  override;

            virtual GraphicElement& SetSize(float size) override;
            virtual float GetSize() const override;

            virtual GraphicElement& SetColor(float r, float g, float b) override;
            virtual GraphicElement& SetColor(const ColorRGB& color) override;
            virtual ColorRGB GetColor() const;

            GraphicElementBase() {}
        protected:
           

            virtual void OnDraw() {};
            virtual GBoundingBox OnGetBBox() const { return {}; };
            virtual void OnScale(float x, float y, float z);
            virtual void OnTranslate(float x, float y, float z);
            virtual void OnRotate(float angle, float x, float y, float z);

        private:
            Resources::ShaderProgramPtr shader_;
            ColorRGB color_ = { 1,0,0 };
            glm::mat4 model_ = glm::identity<glm::mat4>();
            float size_ = 1.f;
        };
	}
}