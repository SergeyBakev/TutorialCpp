#include "pch.h"
#include "GPolygon.h"

namespace Common
{
    namespace Graphic
    {
        void GPolygon::AddPoint(float x, float y, float z)
        {
            vertexes_.push_back(x);
            vertexes_.push_back(y);
            vertexes_.push_back(z);
        }

        void GPolygon::OnDraw()
        {
            GraphicElementBase::OnDraw();
            GLuint vertexbuffer;
            glGenBuffers(1, &vertexbuffer);

            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, vertexes_.size() * sizeof(float), vertexes_.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vertexes_.size() / 3);
        }
    }
}
