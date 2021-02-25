#include "pch.h"
#include "ShaderProgram.h"

namespace Common
{
    namespace Resources
    {
   
        ShaderProgram::~ShaderProgram()
        {
            Destroy();
        }

        ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
        {
            Destroy();
            id_ = other.id_;
            other.id_ = 0;
        }

        void ShaderProgram::SetUniformMatrix4(std::string_view name, const glm::mat4& matrix)
        {
            _ASSERT(IsUsed() != 0);
            glUniformMatrix4fv(glGetUniformLocation(id_, name.data()), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        bool ShaderProgram::GetUniformMatrix(std::string_view name, const glm::mat4& matrix)
        {
            GLint t = glGetUniformLocation(id_, name.data());
            glGetUniformfv(id_, t, (float*)glm::value_ptr(matrix));

            return true;
        }

        GLuint ShaderProgram::GetAtribLocation(std::string_view name) const
        {
            _ASSERT(IsUsed() != 0);
            return glGetAttribLocation(id_, name.data());
        }

        void ShaderProgram::SetAtribLocation(GLuint pos, std::string_view name)
        {
            _ASSERT(IsUsed() != 0);
            if (GetAtribLocation(name) != -1)
                glBindAttribLocation(id_, pos, name.data());
        }

        void ShaderProgram::SetAtribsLocation(std::initializer_list<std::pair<GLuint, std::string>> conventions)
        {
            for (auto& conv : conventions)
                SetAtribLocation(conv.first, conv.second);
        }

        void ShaderProgram::Destroy()
        {
            isDestoroyed_ = true;
            glDeleteShader(id_);
        }

        void ShaderProgram::Use() const
        {
            isUsed_ = true;
            glUseProgram(id_);
        }

        void ShaderProgram::Unuse() const
        {
            glUseProgram(0);
            isUsed_ = false;
        }
        


    }
}
