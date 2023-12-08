#include "VertexBuffer.h"

namespace nc {
	VertexBuffer::VertexBuffer() {
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);
	}

	VertexBuffer::~VertexBuffer() {
		if(this->ibo != NULL) {
			glDeleteBuffers(1, &(this->ibo));
		}
		if(this->vbo != NULL) {
			glDeleteBuffers(1, &(this->vbo));
		}
		if(this->vao != NULL) {
			glDeleteVertexArrays(1, &(this->vao));
		}
	}

	bool VertexBuffer::Create(std::string name, ...) {
		return true;
	}

	void VertexBuffer::CreateVertexBuffer(GLsizei size, GLsizei vertexCount, GLvoid* data) {
		// size is number of bytes of data

		// store vertex count for draw call
		this->vertexCount = vertexCount;

		// create vertex buffer
		glGenBuffers(1, &(this->vbo));
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		// copy data into vertex buffer
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei count, GLvoid* data) {
		// store index type (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT)
		this->indexType = indexType;
		// store index count for draw call
		this->indexCount = count;

		// create index buffer
		glGenBuffers(1, &(this->ibo));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo); // the index buffer will use GL_ELEMENT_ARRAY_BUFFER

		GLsizei size = 0; // index count * number of bytes of type (sizeof() returns the number of bytes in the data type)
		switch(this->indexType) {
			case GL_UNSIGNED_BYTE:
				size = this->indexCount * sizeof(GLubyte);
				break;
			case GL_UNSIGNED_SHORT:
				size = this->indexCount * sizeof(GLushort);
				break;
			case GL_UNSIGNED_INT:
				size = this->indexCount * sizeof(GLuint);
				break;
		}

		// copy data into index buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::SetAttribute(int attribindex, GLint size, GLsizei stride, GLuint offset) {
		// size is number of elements (position = 3 (xyz), color = 3 (rgb), texcoord = 2 (uv))

		// bind vertex buffer
		glBindVertexBuffer(0, this->vbo, 0, stride);
		// enable vertex attribute (position, color, ...)
		glEnableVertexAttribArray(attribindex);
		// set vertex attribute format
		glVertexAttribFormat(attribindex, size, GL_FLOAT, GL_FALSE, offset);
		// bind vertex attribute index to vertex buffer
		glVertexAttribBinding(attribindex, 0);
	}

	void VertexBuffer::Draw(GLenum primitiveType) {
		// bind vertex array object
		glBindVertexArray(this->vao);

		// if index buffer object was set (!= 0) then draw using index array data, else render primitives from vertex array data
		if(this->ibo != NULL) {
			glDrawElements(primitiveType, this->indexCount, this->indexType, 0);
		} else if(this->vbo != NULL) {
			glDrawArrays(primitiveType, 0, this->vertexCount);
		}
	}
}
