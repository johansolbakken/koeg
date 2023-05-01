#pragma once

#include "mesh.h"
#include "shader.h"
#include "cameracontroller.h"
#include "model.h"

class Renderer
{
public:
	static void drawMeshHZ(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform, const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<Hazel::EditorCamera>& camera);
	static void drawModelHZ(const std::shared_ptr<Model>& model, const glm::mat4& transform, const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<Hazel::EditorCamera>& camera);
};