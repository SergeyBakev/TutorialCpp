#pragma once
namespace Common
{
	namespace Graphic
	{
		class GIndexBuffer
		{
		public:
			GIndexBuffer();
			GIndexBuffer(GIndexBuffer&) = delete;
			GIndexBuffer& operator=(GIndexBuffer&) = delete;

			~GIndexBuffer();

			void Atach(const void* data, GLsizei size);
			void Bind() const;
			void Unbind() const;

			GLsizei CountVertexes() const;
		private:
			GLuint id_;
			GLsizei size_;
		};
	}
}