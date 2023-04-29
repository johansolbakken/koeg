#include "window.h"

#include "openglapi.h"
#include <GLFW/glfw3.h>

#include "log.h"
#include "input.h"

Window::Window(const std::string& title, uint32_t width, uint32_t height)
		: m_width(width), m_height(height)
{
	if (!glfwInit())
	{
		LOG_ERROR("Failed to initialize GLFW");
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		LOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL())
	{
		LOG_ERROR("Failed to initialize GLAD");
		exit(-1);
	}

	std::string version = (char*)glGetString(GL_VERSION);
	LOG_INFO("OpenGL version: {}", version);

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			Input::setKeyPressed((KeyCode)key, true);
		}
		else if (action == GLFW_RELEASE)
		{
			Input::setKeyPressed((KeyCode)key, false);
		}
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
	{
		Input::setMousePosition(xpos, ypos);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			Input::setMouseButtonPressed((MouseButton)button, true);
		}
		else if (action == GLFW_RELEASE)
		{
			Input::setMouseButtonPressed((MouseButton)button, false);
		}
	});

	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		Input::setMouseScrollDelta(xoffset, yoffset);
	});

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});
}

Window::~Window()
{
	glfwTerminate();
}

void Window::update()
{
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	GLCall(glViewport(0, 0, width, height));

	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

uint32_t Window::width() const
{
	return m_width;
}

uint32_t Window::height() const
{
	return m_height;
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

void Window::setCursorMode(int mode) const
{
	glfwSetInputMode(m_window, GLFW_CURSOR, mode);
}