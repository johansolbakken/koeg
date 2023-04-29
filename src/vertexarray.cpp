#include "vertexarray.h"

#include "openglapi.h"

uint32_t getSizeOfType(VertexDataType type)
{
	switch (type)
	{
	case VertexDataType::Float:
		return 4;
	case VertexDataType::UnsignedInt:
		return 4;
	case VertexDataType::UnsignedByte:
		return 1;
	}
	return 0;
}

uint32_t getOpenGlType(VertexDataType type)
{
	switch (type)
	{
	case VertexDataType::Float:
		return GL_FLOAT;
	case VertexDataType::UnsignedInt:
		return GL_UNSIGNED_INT;
	case VertexDataType::UnsignedByte:
		return GL_UNSIGNED_BYTE;
	}
	return 0;
}

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_rendererId));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_rendererId));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer,
		const std::vector<VertexLayoutElement>& layout)
{
	bind();

	vertexBuffer->bind();
	uint32_t stride = 0;
	for (const auto& element: layout)
	{
		stride += element.count * getSizeOfType(element.type);
	}

	uint32_t offset = 0;
	for (uint32_t i = 0; i < layout.size(); i++)
	{
		const auto& element = layout[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, getOpenGlType(element.type), element.normalized, stride,
				(const void*)offset));
		offset += element.count * getSizeOfType(element.type);
	}

	unbind();
}

void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	bind();
	indexBuffer->bind();
	m_indexBuffer = indexBuffer;
	unbind();
}

std::shared_ptr<IndexBuffer> VertexArray::indexBuffer() const
{
	return m_indexBuffer;
}