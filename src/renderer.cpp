#include "renderer.h"

#include "log.h"

#include "openglapi.h"

void Renderer::drawMeshHZ(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform,
		const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<Hazel::EditorCamera>& camera)
{
	auto projection = camera->GetViewProjection();
	auto mvp = projection * transform;

	shader->bind();
	mesh->vertexArray()->bind();

	uint32_t numDiffuse = 0;
	uint32_t numSpecular = 0;
	uint32_t i = 0;
	for (const auto& texture: mesh->textures())
	{
		if (texture->type() == TextureType::Diffuse)
		{
			std::string type = textureTypeToString(texture->type());
			auto number = std::to_string(numDiffuse);
			shader->setUniform1i(type + number, i);
			numDiffuse++;
		}
		else if (texture->type() == TextureType::Specular)
		{
			std::string type = textureTypeToString(texture->type());
			auto number = std::to_string(numSpecular);
			shader->setUniform1i(type + number, i);
			numSpecular++;
		}
		else
		{
			LOG_WARN("Texture type not supported: {}", textureTypeToString(texture->type()));
			continue;
		}
		texture->bind(i);
		i++;
	}

	shader->setUniformMat4f("mvp", mvp);
	//shader->setUniform4f("lightColor", lightColor);
	shader->setUniformMat4f("model", transform);
	//shader->setUniform3f("lightPos", lightPos);
	shader->setUniform3f("viewPos", camera->GetPosition());

	glDrawElements(GL_TRIANGLES, (int)mesh->vertexArray()->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}