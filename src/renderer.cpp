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

	uint8_t whiteColor[4] = { 255, 255, 255, 255 };
	auto whiteDiffuse = Texture::create(TextureType::Diffuse, TextureFormat::Rgba, 1, 1, 4, whiteColor);
	auto whiteSpec = Texture::create(TextureType::Specular, TextureFormat::Rgba, 1, 1, 4, whiteColor);

	if (numDiffuse == 0)
	{
		whiteDiffuse->bind(i);
		shader->setUniform1i("diffuse0", i);
		i++;
	}
	if (numSpecular == 0)
	{
		whiteSpec->bind(i);
		shader->setUniform1i("specular0", i);
		i++;
	}


	GLCall(glDrawElements(GL_TRIANGLES, (int)mesh->vertexArray()->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::drawModelHZ(const std::shared_ptr<Model>& model, const glm::mat4& transform,
		const std::shared_ptr<Shader>& shader, const std::shared_ptr<Hazel::EditorCamera>& camera)
{
	for (const auto& mesh: model->meshes())
	{
		drawMeshHZ(mesh, transform, shader, camera);
	}
}