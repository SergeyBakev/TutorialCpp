#include "pch.h"
#include "GIndexArrayObject.h"

namespace Common
{
	namespace Graphic
	{
		GIndexBuffer::GIndexBuffer() : id_(0), size_(0)
		{
		}

		GIndexBuffer::~GIndexBuffer()
		{
			if (id_)
				glDeleteBuffers(1, &id_);
		}

	
		void GIndexBuffer::Atach(const void* data, GLsizei size)
		{
			glGenBuffers(1, &id_);
			glBindBuffer(GL_INDEX_ARRAY, id_);
			glBufferData(GL_INDEX_ARRAY, size, data, GL_DYNAMIC_DRAW);
		}

		void GIndexBuffer::Bind() const
		{
			glBindBuffer(GL_INDEX_ARRAY, id_);
		}

		
		void GIndexBuffer::Unbind() const
		{
			glBindBuffer(GL_INDEX_ARRAY, 0);
		}

		GLsizei GIndexBuffer::CountVertexes() const
		{
			return size_;
		}
	}
}