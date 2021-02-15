#include "pch.h"
#include "Graphic.h"

void print(glm::mat4& m);
namespace Common
{
	namespace Graphic
	{
        using namespace Common::Resources;

		void GraphicElementBase::Draw()
		{
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

          /*  GLuint colors_vbo = 0;
            glGenBuffers(1, &colors_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(color_), &color_, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);*/
            if (shader_ != nullptr)
            {
                shader_->SetMatrix4("model", model_);
                if (!shader_->IsUsed())
                    shader_->Use();
            }
            
            OnDraw();
            //shader_->Unuse();

            glDeleteVertexArrays(1, &vao);
		}

        GraphicElementBase::~GraphicElementBase()
        {
           
        }

        void GraphicElementBase::Scale(float x, float y, float z)
        {
            OnScale(x, y, z);
        }

        void GraphicElementBase::Translate(float x, float y, float z)
        {
            OnTranslate(x,y,z);
        }

        void GraphicElementBase::Rotate(float angle, float x, float y, float z)
        {
            OnRotate(angle,x, y, z);
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

        void GraphicElementBase::MultMatrix(glm::mat4 transform)
        {
            model_ *= transform;
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

        void GraphicElementBase::SetColor(float r, float g, float b)
        {
            color_ = { r,g,b };
        }

        void GraphicElementBase::SetSize(float size)
        {
            size_ = size;
        }
        float GraphicElementBase::GetSize() const
        {
            return size_;
        }
	}
}