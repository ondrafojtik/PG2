#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	const void* m_Data;
	unsigned int m_Size;
public:
	VertexBuffer(const void* data, unsigned int size);
	void RefreshBuffer(const void* new_data);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};