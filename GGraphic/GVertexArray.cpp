#include "pch.h"
#include "GVertexArray.h"

namespace Common
{
	namespace Graphic
	{
		GVertexArray::GVertexArray() : id_(0), size_(0)
		{
			glGenVertexArrays(1, &id_);
		}

		GVertexArray::~GVertexArray()
		{
			glDeleteVertexArrays(1, &id_);
		}


		void GVertexArray::Bind() const
		{
			glBindVertexArray(id_);
		}

		
		void GVertexArray::Unbind() const
		{
			glBindVertexArray(0);
		}

		GLsizei GVertexArray::CountVertexes() const
		{
			return size_;
		}

		void GVertexArray::AddBuffer(const GVertexBuffer& buffer, GVertexBufferLayout& layout)
		{
			Bind();
			buffer.Bind();
		
		}
	}
}