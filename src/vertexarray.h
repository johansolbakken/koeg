#pragma once

#include "buffer.h"

#include <memory>
#include <vector>

enum class VertexDataType
{
	Float,
	UnsignedInt,
	UnsignedByte
};

struct VertexLayoutElement
{
	uint32_t count;
	VertexDataType type;
	uint32_t normalized;
};

class VertexArray
{
public:
	VertexArray();

	~VertexArray();

	void bind() const;

	void unbind() const;

	void addBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::vector<VertexLayoutElement>& layout);

	void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	std::shared_ptr<IndexBuffer> indexBuffer() const;

private:
	uint32_t m_rendererId{};
	std::vector<VertexLayoutElement> m_layout{};
	std::shared_ptr<IndexBuffer> m_indexBuffer{};
};
