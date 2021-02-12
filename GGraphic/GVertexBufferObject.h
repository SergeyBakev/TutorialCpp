#pragma once
namespace Common
{
	namespace Graphic
	{
		struct GVertexBufferLayoutElement
		{
			GLuint count;
			GLenum type;
			GLboolean normalized;
		};

		class GVertexBufferLayout
		{
		public:

			void AddElementFloat(GLuint count, const bool normalize);
			void Reserve(GLsizei count);
			GLuint GetStride() const { return stride_; }
			auto GetElements() { return elements_; }
		private:
			std::vector<GVertexBufferLayoutElement> elements_;
			GLuint stride_ = 0;
		};

		class GVertexBuffer
		{
		public:
			GVertexBuffer();
			GVertexBuffer(GVertexBuffer&) = delete;
			GVertexBuffer& operator=(GVertexBuffer&) = delete;

			~GVertexBuffer();

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