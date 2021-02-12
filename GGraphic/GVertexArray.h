#pragma once
#include "GVertexBufferObject.h"

namespace Common
{
	namespace Graphic
	{
		class GVertexArray
		{
		public:
			GVertexArray();
			GVertexArray(GVertexArray&) = delete;
			GVertexArray& operator=(GVertexArray&) = delete;

			~GVertexArray();
			void Bind() const;
			void Unbind() const;

			GLsizei CountVertexes() const;

			void AddBuffer(const GVertexBuffer& buffer, GVertexBufferLayout& layout);
		private:
			GLuint id_;
			GLsizei size_;
		};
	}
}