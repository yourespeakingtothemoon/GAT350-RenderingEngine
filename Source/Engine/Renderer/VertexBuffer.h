#pragma once

#include "Framework/Resource/Resource.h"

#include <vector>

#include <glad/include/glad/glad.h>

namespace nc {
	class VertexBuffer : public Resource {
		public:
			VertexBuffer();
			virtual ~VertexBuffer();

			bool Create(std::string name, ...) override;
			void Draw(GLenum primitiveType = GL_TRIANGLES);
			void Bind() {glBindVertexArray(this->vao);}

			void CreateVertexBuffer(GLsizei size, GLsizei vertexCount, GLvoid* data);
			void CreateIndexBuffer(GLenum indexType, GLsizei count, GLvoid* data);

			void SetAttribute(int index, GLint size, GLsizei stride, GLuint offset);

		protected:
			GLuint vao = 0;			// vertex array object

			GLuint vbo = 0;			// vertex buffer object
			GLuint vertexCount = 0;	// number of vertices in vertex buffer

			GLuint ibo = 0;			// index buffer object
			GLuint indexCount = 0;	// number of indices index buffer
			GLenum indexType = 0;	// data type of index
	};
}