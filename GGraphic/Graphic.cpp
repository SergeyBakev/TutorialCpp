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

        GraphicElementPtr GraphicElementBase::Scale(float x, float y, float z)
        {
            OnScale(x, y, z);
            return shared_from_this();
        }

       GraphicElementPtr GraphicElementBase::Scale(float scaleFactor)
        {
            Scale(scaleFactor, scaleFactor, scaleFactor);
            return shared_from_this();
        }

       GraphicElementPtr GraphicElementBase::Translate(float x, float y, float z)
        {
            OnTranslate(x,y,z);
            return shared_from_this();
        }

       GraphicElementPtr GraphicElementBase::Rotate(float angle, float x, float y, float z)
        {
            OnRotate(angle,x, y, z);
            return shared_from_this();
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

       GraphicElementPtr GraphicElementBase::MultMatrix(glm::mat4 transform)
        {
            model_ *= transform;
            return shared_from_this();
        }

        bool GraphicElementBase::OnScale(float x, float y, float z)
        {
            model_ *= glm::scale(glm::vec3(x, y, z));
            return true;
        }

        bool GraphicElementBase::OnTranslate(float x, float y, float z)
        {
            model_ = glm::translate(model_, { x,y,z });
            return true;
        }

        bool GraphicElementBase::OnRotate(float angle, float x, float y, float z)
        {
            model_ *= glm::rotate(0.5f, glm::vec3(1, 0, 0));
            return true;
        }

       GraphicElementPtr GraphicElementBase::SetColor(float r, float g, float b)
        {
            color_ = { r,g,b };
            return shared_from_this();
        }

       GraphicElementPtr GraphicElementBase::SetColor(const ColorRGB& color)
        {
            color_ = color;
            return shared_from_this();
        }

        ColorRGB GraphicElementBase::GetColor() const
        {
            return color_;
        }

       GraphicElementPtr GraphicElementBase::SetSize(float size)
        {
            size_ = size;
            return shared_from_this();
        }
        float GraphicElementBase::GetSize() const
        {
            return size_;
        }
	}
}