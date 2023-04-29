#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"

#include "shader.h"
#include "vertexarray.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main()
{
	LOG_INFO("Le game");

	if (!glfwInit())
	{
		LOG_ERROR("Failed to initialize GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL())
	{
		LOG_ERROR("Failed to initialize GLAD");
		return -1;
	}

	std::string version = (char*)glGetString(GL_VERSION);
	LOG_INFO("OpenGL version: {}", version);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	float pyramidVertices[] = {
			-0.5, 0.0, 0.5, 0.83, 0.70, 0.44, 0.0, 0.0,
			-0.5, 0.0, -0.5, 0.83, 0.70, 0.44, 5.0, 0.0,
			0.5, 0.0, -0.5, 0.83, 0.70, 0.44, 0.0, 0.0,
			0.5, 0.0, 0.5, 0.83, 0.70, 0.44, 5.0, 0.0,
			0.0, 0.8, 0.0, 0.92, 0.86, 0.76, 2.5, 5.0
	};

	auto pyramidIndices = new uint32_t[18]{
			0, 1, 2,
			2, 3, 0,
			0, 4, 1,
			1, 4, 2,
			2, 4, 3,
			3, 4, 0
	};

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
			2, 3, 0
	};

	Shader shader("shaders/default.vert", "shaders/default.frag");
	shader.bind();

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

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		auto view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));

		auto projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

		auto mvp = projection * view * model;

		texture.bind();
		shader.bind();
		shader.setUniform1i("texture1", 0);
		shader.setUniformMat4f("mvp", mvp);
		pyramidVao->bind();
		glDrawElements(GL_TRIANGLES, (int)pyramidVao->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}