#include "buffer.h"

#include "openglapi.h"

// ------------------------------------------------------------------
// VertexBuffer
// ------------------------------------------------------------------

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
{
	GLCall(glGenBuffers(1, &m_rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
			vertices.data(), GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererId));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

std::shared_ptr<VertexBuffer> VertexBuffer::create(const std::vector<Vertex>& vertices)
{
	return std::make_shared<VertexBuffer>(vertices);
}

// ------------------------------------------------------------------
// IndexBuffer
// ------------------------------------------------------------------

IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices)
		: m_count(indices.size())
{
	GLCall(glGenBuffers(1, &m_rendererId));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
			indices.data(), GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererId));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

uint32_t IndexBuffer::count() const
{
	return m_count;
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(const std::vector<uint32_t>& indices)
{
	return std::make_shared<IndexBuffer>(indices);
}