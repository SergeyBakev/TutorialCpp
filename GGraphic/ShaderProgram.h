#pragma once
namespace Common
{
    namespace Resources
    {
        class ShaderProgram : std::enable_shared_from_this<ShaderProgram>
        {
        public:
            ~ShaderProgram();
           

            ShaderProgram() = delete;
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram& operator=(const ShaderProgram&) = delete;

            ShaderProgram(ShaderProgram&& other) noexcept;

            void SetUniformf(std::string_view name, float value);
            void SetUniform3f(std::string_view name, const glm::vec3& value);
            void SetUniformMatrix4(std::string_view name, const glm::mat4& matrix);
            bool GetUniformMatrix(std::string_view name, const glm::mat4& matrix);

            GLuint GetAtribLocation(std::string_view name) const;

            void SetAtribLocation(GLuint pos, std::string_view name);

            void SetAtribsLocation(std::initializer_list < std::pair<GLuint, std::string>> conventions);

            void Destroy();

            void Use() const;
            void Unuse() const;

            GLuint Id() const { return id_; }
            bool IsValid() const { return !IsDestoroyed() && id_ != 0; }
            bool IsDestoroyed() const { return isDestoroyed_; }
            bool IsUsed() const { return isUsed_; }

            ShaderProgram(GLuint progID, GLuint vertexShID, GLuint fragmendShID) : id_(progID), vetexShId_(vertexShID), fragmentShId_(fragmendShID)
            {

            }

        private:
            GLint GetUniformLocation(std::string_view name) const;
        private:
            GLuint id_ = 0;
            GLuint vetexShId_ = 0;
            GLuint fragmentShId_ = 0;
            mutable bool isUsed_ = false;
            bool isDestoroyed_ = false;
        };

        using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
    }
}
