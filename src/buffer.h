#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec3 color{};
	glm::vec2 texCoords{};
};

class VertexBuffer
{
public:
	VertexBuffer(const std::vector<Vertex>& vertices);

	~VertexBuffer();

	void bind() const;

	void unbind() const;

	static std::shared_ptr<VertexBuffer> create(const std::vector<Vertex>& vertices);

private:
	uint32_t m_rendererId;
};

class IndexBuffer
{
public:
	IndexBuffer(const std::vector<uint32_t>& indices);

	~IndexBuffer();

	void bind() const;

	void unbind() const;

	uint32_t count() const;

	static std::shared_ptr<IndexBuffer> create(const std::vector<uint32_t>& indices);

private:
	uint32_t m_rendererId;
	uint32_t m_count;
};