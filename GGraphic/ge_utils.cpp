#include "pch.h"
#include "ge_utils.h"
#include "GVertexBufferObject.h"
namespace Common
{
	namespace utils
	{
		bool draw_array(const glm3Vectors& points, GLenum drawMode)
		{
            Graphic::GVertexBuffer v;
            v.Atach(points.data(), (GLsizei)(points.size() * sizeof(glm::vec3)));
            v.Bind();
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);
            glDrawArrays(drawMode, 0, (GLsizei)(points.size()));
            v.Unbind();

            return true;
		}
	}
}
