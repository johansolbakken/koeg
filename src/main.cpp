#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "log.h"

#include <string>

#include "shader.h"
#include "vertexarray.h"

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

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL())
    {
        LOG_ERROR("Failed to initialize GLAD");
        return -1;
    }

    std::string version = (char*)glGetString(GL_VERSION);
    LOG_INFO("OpenGL version: {}", version);

    glViewport(0, 0, 800, 600);

    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    uint32_t indices[] = {
            0, 1, 2
    };

    Shader shader("shaders/default.vert", "shaders/default.frag");
    shader.bind();

    auto vao = std::make_shared<VertexArray>();
    auto vbo = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    vao->addBuffer(vbo);

    auto ebo = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        vao->bind();
        ebo->bind();
        glDrawElements(GL_TRIANGLES, (int) ebo->count(), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}