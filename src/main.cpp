#include "openglapi.h"

#include "log.h"

#include "shader.h"
#include "vertexarray.h"
#include "texture.h"
#include "cameracontroller.h"
#include "window.h"
#include "mesh.h"
#include "renderer.h"
#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

double getTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch())
				   .count() /
		   1000.0;
}

extern Window* inputWindow;


int main()
{
	LOG_INFO("Le game");

	Window window("Le game", 1280, 1280 * 9 / 16);
	inputWindow = &window;

	uint8_t whiteColor[4] = { 255, 255, 255, 255 };
	auto whiteDiff = Texture::create(TextureType::Diffuse, TextureFormat::Rgba, 1, 1, 4, whiteColor);
	auto whiteSpec = Texture::create(TextureType::Specular, TextureFormat::Rgba, 1, 1, 4, whiteColor);
	auto texture = Texture::create(TextureType::Diffuse, TextureFormat::Rgba, "textures/popcat.jpg");
	auto brick = Texture::create(TextureType::Diffuse, TextureFormat::Rgba, "textures/brick.jpeg");
	auto planks = Texture::create(TextureType::Diffuse, TextureFormat::Rgba, "textures/planks.png");
	auto planksSpec = Texture::create(TextureType::Specular, TextureFormat::Red, "textures/planksSpec.png");

	std::vector<Vertex> vertices = { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
			Vertex{ glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec2(0.0f, 0.0f) },
			Vertex{ glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec2(0.0f, 1.0f) },
			Vertex{ glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec2(1.0f, 1.0f) },
			Vertex{ glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec2(1.0f, 0.0f) }
	};

// Indices for vertices order
	std::vector<uint32_t> indices = {
			0, 1, 2,
			0, 2, 3
	};

	std::vector<Vertex> lightVertices = { //     COORDINATES     //
			Vertex{ glm::vec3(-0.1f, -0.1f, 0.1f) },
			Vertex{ glm::vec3(-0.1f, -0.1f, -0.1f) },
			Vertex{ glm::vec3(0.1f, -0.1f, -0.1f) },
			Vertex{ glm::vec3(0.1f, -0.1f, 0.1f) },
			Vertex{ glm::vec3(-0.1f, 0.1f, 0.1f) },
			Vertex{ glm::vec3(-0.1f, 0.1f, -0.1f) },
			Vertex{ glm::vec3(0.1f, 0.1f, -0.1f) },
			Vertex{ glm::vec3(0.1f, 0.1f, 0.1f) }
	};

	std::vector<uint32_t> lightIndices = {
			0, 1, 2,
			0, 2, 3,
			0, 4, 7,
			0, 7, 3,
			3, 7, 6,
			3, 6, 2,
			2, 6, 5,
			2, 5, 1,
			1, 5, 4,
			1, 4, 0,
			4, 5, 6,
			4, 6, 7
	};

	auto cube = Mesh::create(lightVertices, lightIndices, {});
	auto plane = Mesh::create(vertices, indices, { planks, planksSpec });

	auto shader = Shader::create("shaders/default.vert", "shaders/default.frag");
	auto lightShader = Shader::create("shaders/light.vert", "shaders/light.frag");

	auto model = Model::create("vendor/glTF-Sample-Models/2.0/Avocado/glTF/Avocado.gltf");

	// Texture

/*
	auto camera = std::make_shared<Camera>();
	camera->setPositionWithLookAt(glm::vec3(-2.0f, 2.0f, -2.0f), glm::vec3(0.0f));
	auto cameraController = std::make_shared<CameraController>(camera);*/

	auto editorCamera = std::make_shared<Hazel::EditorCamera>();
	editorCamera->SetViewportSize(window.width(), window.height());

	glm::vec3 lightPos(0.5f, 0.5f, 0.5f);
	glm::vec4 lightColor(1.0f, 1.0f, 1.0f, 1.0f);
	auto lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glEnable(GL_DEPTH_TEST);

	double lastTime = getTime();
	while (!window.shouldClose())
	{
		double currentTime = getTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		editorCamera->OnUpdate(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Plane
		auto transform = glm::mat4(1.0f);
		transform = glm::scale(transform, glm::vec3(10.0f));
		shader->bind();
		shader->setUniform4f("lightColor", lightColor);
		shader->setUniform3f("lightPos", lightPos);
		// 3Renderer::drawMeshHZ(plane, transform, shader, editorCamera);
		Renderer::drawModelHZ(model, transform, shader, editorCamera);

		// Shader cube
		lightShader->bind();
		lightShader->setUniform4f("lightColor", lightColor);
		Renderer::drawMeshHZ(cube, lightModel, lightShader, editorCamera);

		window.update();
	}
	return 0;
}