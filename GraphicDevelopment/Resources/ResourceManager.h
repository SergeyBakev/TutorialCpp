#pragma once
#include "ShaderProgram.h"
#include "Logger.h"

namespace Common
{
	namespace Resources
	{
        std::string LoadFileCode(std::string_view file);

        class ResourceManager
        {
        public:
            using ShaderProgramMap = std::map<std::string, ShaderProgramPtr>;

            ResourceManager(const ResourceManager&) = delete;
            ResourceManager(ResourceManager&&) = delete;
            ResourceManager& operator=(const ResourceManager&) = delete;
            ResourceManager& operator=(ResourceManager&&) = delete;

            static ResourceManager* Instance();
          
            void InjectLogger(ILogger* log);

            static ShaderProgramPtr SCreateShader(const std::string& shaderName, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
        

            static void SDestory();

            void Destory();

            ShaderProgramPtr CreateShader(const std::string& shaderName, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

            ShaderProgramPtr GetShader(const std::string& shaderName);

        private:

            

        private:
            ResourceManager() = default;
            ShaderProgramMap shaders_map_;

        };
	}
}