#include "openglapi.h"

#include "log.h"

#include "shader.h"
#include "vertexarray.h"
#include "texture.h"
#include "cameracontroller.h"
#include "window.h"

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

	// cube
	float cubeVertices[] = {
			-0.1, -0.1, 0.1,
			0.1, -0.1, 0.1,
			0.1, 0.1, 0.1,
			-0.1, 0.1, 0.1,
			-0.1, -0.1, -0.1,
			0.1, -0.1, -0.1,
			0.1, 0.1, -0.1,
			-0.1, 0.1, -0.1
	};

	uint32_t cubeIndices[] = {
			0, 1, 2,
			2, 3, 0,
			1, 5, 6,
			6, 2, 1,
			7, 6, 5,
			5, 4, 7,
			4, 0, 3,
			3, 7, 4,
			4, 5, 1,
			1, 0, 4,
			3, 2, 6,
			6, 7, 3
	};

	auto cubeVao = std::make_shared<VertexArray>();
	auto cubeVbo = std::make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
	auto cubeIbo = std::make_shared<IndexBuffer>(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
	cubeVao->addBuffer(cubeVbo, {{ 3, VertexDataType::Float }});
	cubeVao->setIndexBuffer(cubeIbo);

	// pyramid

	float pyramidVertices[] = {
			-0.5, 0.0, 0.5, 0.83, 0.70, 0.44, 0.0, 0.0,
			-0.5, 0.0, -0.5, 0.83, 0.70, 0.44, 5.0, 0.0,
			0.5, 0.0, -0.5, 0.83, 0.70, 0.44, 0.0, 0.0,
			0.5, 0.0, 0.5, 0.83, 0.70, 0.44, 5.0, 0.0,
			0.0, 0.8, 0.0, 0.92, 0.86, 0.76, 2.5, 5.0 };

	auto pyramidIndices = new uint32_t[18]{
			0, 1, 2,
			2, 3, 0,
			0, 4, 1,
			1, 4, 2,
			2, 4, 3,
			3, 4, 0 };

	auto pyramidVao = std::make_shared<VertexArray>();
	auto pyramidVbo = std::make_shared<VertexBuffer>(pyramidVertices, sizeof(pyramidVertices));
	auto pyramidIbo = std::make_shared<IndexBuffer>(pyramidIndices, 18);
	pyramidVao->addBuffer(pyramidVbo, {
			{ 3, VertexDataType::Float },
			{ 3, VertexDataType::Float },
			{ 2, VertexDataType::Float }
	});
	pyramidVao->setIndexBuffer(pyramidIbo);

	float vertices[] = {
			// pos, color, tex
			-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 0.0, 0.0,
			-0.5f, 0.5f, 0.0f, 0.0, 1.0, 0.0, 0.0, 1.0,
			0.5f, 0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0, 1.0,
			0.5f, -0.5f, 0.0f, 1.0, 1.0, 1.0, 1.0, 0.0
	};

	uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0 };

	Shader shader("shaders/default.vert", "shaders/default.frag");
	Shader lightShader("shaders/light.vert", "shaders/light.frag");

	auto vao = std::make_shared<VertexArray>();
	auto vbo = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
	vao->addBuffer(vbo, {
			{ 3, VertexDataType::Float, false },
			{ 3, VertexDataType::Float, false },
			{ 2, VertexDataType::Float, false }
	});

	auto ebo = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
	vao->setIndexBuffer(ebo);

	// Texture
	Texture texture("textures/popcat.jpg");
	Texture brick("textures/brick.jpeg");

	auto camera = std::make_shared<Camera>();
	auto cameraController = std::make_shared<CameraController>(camera);

	glEnable(GL_DEPTH_TEST);

	double lastTime = getTime();
	while (!window.

			shouldClose()

			)
	{
		double currentTime = getTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		cameraController->update(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			auto model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)getTime() / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

			auto view = camera->viewMatrix();
			auto projection = camera->projectionMatrix();
			auto mvp = projection * view * model;

			texture.

					bind();

			shader.

					bind();

			shader.setUniform1i("texture1", 0);
			shader.setUniformMat4f("mvp", mvp);
			pyramidVao->bind();

			glDrawElements(GL_TRIANGLES, (int)pyramidVao->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
		}

		{
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
			auto view = camera->viewMatrix();
			auto projection = camera->projectionMatrix();
			auto mvp = projection * view * model;

			lightShader.bind();
			lightShader.setUniformMat4f("mvp", mvp);
			cubeVao->bind();

			glDrawElements(GL_TRIANGLES, (int)cubeVao->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
		}

		window.update();
	}
	return 0;
}