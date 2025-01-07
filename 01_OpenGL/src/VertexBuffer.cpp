#include "VertexBuffer.h"
#include "GLcall.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : m_Data(data), m_Size(size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_Size, m_Data, GL_STATIC_DRAW));
}

void VertexBuffer::RefreshBuffer(const void* new_data)
{
	//try to do it with the offsetof(float[2]) prob.

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_Size, new_data);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, m_Size, new_data, GL_STATIC_DRAW);

	//m_Data = new_data;
	//GLCall(glGenBuffers(1, &m_RendererID));
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	//GLCall(glBufferData(GL_ARRAY_BUFFER, m_Size, m_Data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
