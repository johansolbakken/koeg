#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "mesh.h"

class Model
{
public:
	Model(const std::string& path);

	~Model();

	static std::shared_ptr<Model> create(const std::string& path);

	[[nodiscard]] const auto& meshes() const
	{
		return m_meshes;
	}

private:
	std::vector<std::shared_ptr<Mesh>> m_meshes;
};