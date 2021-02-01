#include "stdafx.h"
#include "ResourceManager.h"

namespace Common
{
	namespace Resources
	{
        class ShaderLoader
        {
        public:
            ShaderLoader() = default;
            ShaderLoader(const ShaderLoader&) = delete;
            ShaderLoader(ShaderLoader&&) = delete;
            ShaderLoader& operator=(const ShaderLoader&) = delete;
            ShaderLoader& operator=(ShaderLoader&&) = delete;

            ShaderLoader(const std::string& vertexShaderFile, const std::string& fragmentFileShader)
            {
                Load(vertexShaderFile, fragmentFileShader);
            }

            bool Load(const std::string& vertexShaderFile, const std::string& fragmentFileShader)
            {
                auto vrtxSHCode = LoadFileCode(vertexShaderFile);
                auto frgmntSHCode = LoadFileCode(fragmentFileShader);
                GLuint vrtxId;
                GLuint frgmntId;
                if (!CreateAndCompileShader(vrtxSHCode, GL_VERTEX_SHADER, vrtxId))
                    return false;

                if (!CreateAndCompileShader(frgmntSHCode, GL_FRAGMENT_SHADER, frgmntId))
                {
                    glDeleteShader(vrtxId);
                    return false;
                }

                GLuint prgrmId = glCreateProgram();
                if (!prgrmId)
                    return false;

                glAttachShader(prgrmId, vrtxId);
                glAttachShader(prgrmId, frgmntId);
                glLinkProgram(prgrmId);
                if (!CheckLinkError(prgrmId))
                {
                    glDeleteProgram(prgrmId);
                    return false;
                }

                shader_ = ShaderProgramPtr(new ShaderProgram(prgrmId, vrtxId, frgmntId));
                return true;
            }

            static ShaderProgramPtr LoadShader(const std::string& vertexShaderFile, const std::string& fragmentFileShader)
            {
                ShaderLoader loader(vertexShaderFile, fragmentFileShader);
                return loader.GetShader();
            }


            ~ShaderLoader()
            {
                if (!IsAtached())
                    glDeleteProgram(shader_->Id());
            }

            ShaderProgramPtr GetShader() const { isDetached_ = true; return shader_; }

            bool HasError() const { return IsLoaded() && IsCompiled() && IsLinked(); }
            bool IsCompiled() const { return isCompiled_; }
            bool IsLinked() const { return isCompiled_; }
            bool IsLoaded() const { return isLoaded_; }
            bool IsAtached() const { return isDetached_; }

        private:

            bool CreateAndCompileShader(const std::string& shaderCode, const GLenum shaderType, GLuint& id)
            {
                if (shaderCode.empty())
                {
                    isLoaded_ = false;
                    return false;
                }

                id = glCreateShader(shaderType);
                if (id == 0)
                    return false;

                const char* code = shaderCode.c_str();
                glShaderSource(id, 1, &code, nullptr);
                glCompileShader(id);
                return CheckComplieErrors(id);
            }

            bool CheckComplieErrors(GLuint shaderId) const
            {
                GLint status_compile;
                glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status_compile);
                if (!status_compile)
                {
                    isCompiled_ = false;
                    GLchar log[1024];
                    GLsizei msg_size = 0;
                    glGetShaderInfoLog(shaderId, sizeof(log), &msg_size, log);
                    std::string errors;
                    errors.assign(log, log + msg_size);
                    std::cerr << "ERROR::SHADER: Compile time error:\n" << std::endl;
                    std::cerr << errors << std::endl;
                    return false;
                }
                isCompiled_ = true;
                return true;
            }

            bool CheckLinkError(GLuint programId) const
            {
                GLint link_status;
                glGetProgramiv(programId, GL_LINK_STATUS, &link_status);
                if (!link_status)
                {
                    isCompiled_ = false;
                    GLchar log[1024];
                    GLsizei msg_size = 0;
                    glGetShaderInfoLog(programId, sizeof(log), &msg_size, log);
                    std::string errors;
                    errors.assign(log, log + msg_size);
                    std::cerr << "ERROR::SHADER: Link time error:\n" << std::endl;
                    std::cerr << errors << std::endl;
                    return false;
                }

                isLinked_ = true;
                return true;
            }

        private:
            ShaderProgramPtr shader_;
            mutable bool isCompiled_ = false;
            mutable bool isLinked_ = false;
            mutable bool isLoaded_ = false;
            mutable bool isDetached_ = false;
        };

		std::string LoadFileCode(std::string_view file)
		{
			std::stringstream ss;
			std::ifstream fs;
			fs.open(file.data());
			if (!fs.is_open())
				return {};

			ss << fs.rdbuf();
			return ss.str();
		}

		ResourceManager* ResourceManager::Instance()
		{
			static ResourceManager manager;
			return &manager;
		}

		void ResourceManager::InjectLogger(ILogger* log)
		{
		}
		ShaderProgramPtr ResourceManager::SCreateShader(const std::string& shaderName, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
		{
			return Instance()->CreateShader(shaderName, vertexShaderFilePath, fragmentShaderFilePath);
		}

		void ResourceManager::SDestory()
		{
			Instance()->Destory();
		}

		void ResourceManager::Destory()
		{
			for (auto& it : shaders_map_)
				it.second->Destroy();

			shaders_map_.clear();
		}

		ShaderProgramPtr ResourceManager::CreateShader(const std::string& shaderName, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
		{
			ShaderLoader loader(vertexShaderFilePath, fragmentShaderFilePath);
			if (loader.HasError())
			{
				//log write error
				return nullptr;
			}
			shaders_map_.insert({ shaderName,loader.GetShader() });
			return loader.GetShader();
		}

		ShaderProgramPtr ResourceManager::GetShader(const std::string& shaderName)
		{
			const auto& it = shaders_map_.find(shaderName);

			return std::end(shaders_map_) != it ? shaders_map_[shaderName] : nullptr;
		}
	
	}
}
