#pragma once

#include <string>

struct GLFWwindow;

class Window
{
public:
	Window(const std::string& title, uint32_t width, uint32_t height);

	~Window();

	void update();

	uint32_t width() const;

	uint32_t height() const;

	bool shouldClose() const;

	void setCursorMode(int mode) const;

private:
	GLFWwindow* m_window;
	uint32_t m_width;
	uint32_t m_height;
};