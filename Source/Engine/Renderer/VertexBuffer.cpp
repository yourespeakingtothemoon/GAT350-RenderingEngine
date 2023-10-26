#include "VertexBuffer.h"

namespace nc
{
	VertexBuffer::VertexBuffer()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_ibo) glDeleteBuffers(1, &m_ibo);
		if (m_vbo) glDeleteBuffers(1, &m_vbo);
		if (m_vao) glDeleteVertexArrays(1, &m_vao);
	}

	bool VertexBuffer::Create(std::string name, ...)
	{
		return true;
	}

	void VertexBuffer::CreateVertexBuffer(GLsizei size, GLsizei vertexCount, GLvoid* data)
	{
		m_vertexCount = vertexCount;
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei count, GLvoid* data)
	{
		m_indexType = indexType;
		m_indexCount = count;

		
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		GLsizei size = 0; 
		switch (m_indexType)
		{
		case GL_UNSIGNED_BYTE:
			size = m_indexCount * sizeof(GLubyte);
			break;
		case GL_UNSIGNED_SHORT:
			size = m_indexCount * sizeof(GLushort);
			break;
		case GL_UNSIGNED_INT:
			size = m_indexCount * sizeof(GLuint);
			break;
		}

		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::SetAttribute(int attribindex, GLint size, GLsizei stride, GLuint offset)
	{
		glBindVertexBuffer(0, m_vbo, 0, stride);
		glEnableVertexAttribArray(attribindex);
		glVertexAttribFormat(attribindex, size, GL_FLOAT, GL_FALSE, offset);
		glVertexAttribBinding(attribindex, 0);
	}

	void VertexBuffer::Draw(GLenum primitiveType)
	{
		glBindVertexArray(m_vao);
		if (m_ibo)
		{
			glDrawElements(primitiveType, m_indexCount, m_indexType, 0);
		}
		else if (m_vbo)
		{
			glDrawArrays(primitiveType, 0, m_vertexCount);
		}
	}
}
