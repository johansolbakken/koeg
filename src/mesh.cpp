#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
		const std::vector<std::shared_ptr<Texture>>& textures)
		: m_vertices(vertices), m_indices(indices), m_textures(textures)
{
	m_vertexArray = VertexArray::create();
	auto vertexBuffer = VertexBuffer::create(m_vertices);
	m_vertexArray->addBuffer(vertexBuffer, {
			{ 3, VertexDataType::Float },
			{ 3, VertexDataType::Float },
			{ 3, VertexDataType::Float },
			{ 2, VertexDataType::Float }
	});
	auto indexBuffer = IndexBuffer::create(m_indices);
	m_vertexArray->setIndexBuffer(indexBuffer);
}

Mesh::~Mesh()
{
}

std::shared_ptr<Mesh> Mesh::create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,
		const std::vector<std::shared_ptr<Texture>>& textures)
{
	return std::make_shared<Mesh>(vertices, indices, textures);
}