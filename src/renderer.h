#pragma once

#include "mesh.h"
#include "shader.h"
#include "cameracontroller.h"

class Renderer
{
public:
	static void drawMeshHZ(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform, const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<Hazel::EditorCamera>& camera);
};