#include "stdafx.h"
#include "GVertexBufferObject.h"

namespace Common
{
	namespace Graphic
	{
		GVertexBuffer::GVertexBuffer() : id_(0), size_(0)
		{
		}

		GVertexBuffer::~GVertexBuffer()
		{
			Unbind();
			if (id_)
				glDeleteBuffers(1, &id_);
		}

	
		void GVertexBuffer::Atach(const void* data, GLsizei size)
		{
			glGenBuffers(1, &id_);
			glBindBuffer(GL_ARRAY_BUFFER, id_);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		}

		void GVertexBuffer::Bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, id_);
		}

		
		void GVertexBuffer::Unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		GLsizei GVertexBuffer::CountVertexes() const
		{
			return size_;
		}

		
		void GVertexBufferLayout::AddElementFloat(GLuint count, const bool normalize)
		{
			elements_.push_back({ count,GL_FLOAT,normalize });
			stride_ = count * sizeof(GL_FLOAT);
		}

		void GVertexBufferLayout::Reserve(GLsizei count)
		{
			elements_.reserve(count);
		}

	}
}