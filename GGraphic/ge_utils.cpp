#include "pch.h"
#include "ge_utils.h"
#include "GVertexBufferObject.h"
namespace Common
{
	namespace utils
	{
		bool draw_array(const glm3Vectors& points, GLenum drawMode)
		{
            ColorRGB r = { 1.0,0.0,0.0 };
            return draw_array(points, drawMode, r);
		}

        bool draw_array(const glm3Vectors& points, GLenum drawMode, const ColorRGB& color)
        {
            
            Graphic::GVertexBuffer v;
            v.Atach(points.data(), (GLsizei)(points.size() * sizeof(glm::vec3)));

            std::vector<ColorRGB> colors;
            colors.push_back(color);
            colors.push_back(color);
            colors.push_back(color);

            Graphic::GVertexBuffer c;
            c.Atach(colors.data(), (GLsizei)(colors.size() * sizeof(color)));

            

            v.Bind();
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);

            
            c.Bind();
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);

            glDrawArrays(drawMode, 0, (GLsizei)(points.size()));

            v.Unbind();
            c.Unbind();

            return true;
        }

        bool draw_array_object(const glm3Vectors& points, GLenum drawMode)
        {
            
            ColorRGB r = { 1.0,0.0,0.0 };
            return draw_array_object(points,drawMode,r);
        }

        bool draw_array_object(const glm3Vectors& points, GLenum drawMode, const ColorRGB& color)
        {
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            draw_array(points, drawMode, color);

            glBindVertexArray(0);
            glDeleteVertexArrays(1, &vao);
            return true;
        }

        bool draw_line(const glm::vec3& p0, const glm::vec3& p1)
        {
            return draw_line(p0, p1, { 1,1,1 });
        }

        bool draw_line(const glm::vec3& p0, const glm::vec3& p1, const ColorRGB& color)
        {
            return draw_array_object({ p0,p1 }, GL_LINES, color);
        }

        glm::vec4 to_vec4(const glm::vec3& v)
        {
            return glm::vec4(v,1);
        }

        glm::vec3 to_vec3(const glm::vec4& v)
        {
            return { v.x,v.y,v.z };
        }
	}
}
