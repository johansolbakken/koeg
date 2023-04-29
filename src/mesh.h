#pragma once

#include <memory>
#include <vector>

#include "vertexarray.h"
#include "texture.h"

#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices,
			const std::vector<std::shared_ptr<Texture>>& textures);

	~Mesh();

	static std::shared_ptr<Mesh> create(const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices,
			const std::vector<std::shared_ptr<Texture>>& textures);

	const std::shared_ptr<VertexArray>& vertexArray() const
	{
		return m_vertexArray;
	}

	const std::vector<std::shared_ptr<Texture>>& textures() const
	{
		return m_textures;
	}

private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<std::shared_ptr<Texture>> m_textures;

	std::shared_ptr<VertexArray> m_vertexArray;
};