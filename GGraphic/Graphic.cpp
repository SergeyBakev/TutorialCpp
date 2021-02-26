#include "pch.h"
#include "Graphic.h"
#include "GVertexBufferObject.h"

void print(glm::mat4& m);
namespace Common
{
	namespace Graphic
	{
        using namespace Common::Resources;

		void GraphicElementBase::Draw()
		{          
            if (shader_ != nullptr)
            {
                shader_->SetUniformMatrix4("model", model_);
                if (!shader_->IsUsed())
                    shader_->Use();
            }
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            std::vector<ColorRGB> colors;
            colors.push_back(color_);
            colors.push_back(color_);
            colors.push_back(color_);
            Graphic::GVertexBuffer c;
            c.Atach(colors.data(), (GLsizei)(colors.size() * sizeof(color_)));
            c.Bind();
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);
            glLineWidth(GetSize());
            OnDraw();
            
            //c.Unbind();
            glBindVertexArray(0);
            glDeleteVertexArrays(1, &vao);
		}

        GraphicElementBase::~GraphicElementBase()
        {
           
        }

        GraphicElement& GraphicElementBase::Scale(float x, float y, float z)
        {
            OnScale(x, y, z);
            return *this;
        }

        GraphicElement& GraphicElementBase::Scale(float scaleFactor)
        {
            Scale(scaleFactor, scaleFactor, scaleFactor);
            return *this;
        }

        GraphicElement& GraphicElementBase::Translate(float x, float y, float z)
        {
            OnTranslate(x,y,z);
            return *this;
        }

        GraphicElement& GraphicElementBase::Rotate(float angle, float x, float y, float z)
        {
            OnRotate(angle,x, y, z);
            return *this;
        }

        glm::mat4 GraphicElementBase::GetTransofrm() const
        {
            return model_;
        }

        GBoundingBox GraphicElementBase::GetBBox() const
        {
            return OnGetBBox();
        }

        void GraphicElementBase::SetActiveShader(const ShaderProgramPtr& shader)
        {
            shader_ = shader;
        }

        ShaderProgramPtr GraphicElementBase::GetActiveShader() const
        {
            return shader_;
        }

        GraphicElement& GraphicElementBase::MultMatrix(glm::mat4 transform)
        {
            model_ *= transform;
            return *this;
        }

        void GraphicElementBase::OnScale(float x, float y, float z)
        {
            model_ *= glm::scale(glm::vec3(x, y, z));
        }

        void GraphicElementBase::OnTranslate(float x, float y, float z)
        {
            model_ = glm::translate(model_, { x,y,z });
        }

        void GraphicElementBase::OnRotate(float angle, float x, float y, float z)
        {
            model_ *= glm::rotate(0.5f, glm::vec3(1, 0, 0));
            print(model_);
        }

        GraphicElement& GraphicElementBase::SetColor(float r, float g, float b)
        {
            color_ = { r,g,b };
            return *this;
        }

        GraphicElement& GraphicElementBase::SetColor(const ColorRGB& color)
        {
            color_ = color;
            return *this;
        }

        ColorRGB GraphicElementBase::GetColor() const
        {
            return color_;
        }

        GraphicElement& GraphicElementBase::SetSize(float size)
        {
            size_ = size;
            return *this;
        }
        float GraphicElementBase::GetSize() const
        {
            return size_;
        }
	}
}