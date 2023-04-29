#include <glad/glad.h>
#include "openglapi.h"
#include <GLFW/glfw3.h>

#include "log.h"

#include "shader.h"
#include "vertexarray.h"
#include "texture.h"

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

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture.bind();
		shader.bind();
		shader.setUniform1i("texture1", 0);
		vao->bind();
		glDrawElements(GL_TRIANGLES, (int)vao->indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}