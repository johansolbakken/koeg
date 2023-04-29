#include "shader.h"

#include <glad/glad.h>

#include <fstream>
#include <iostream>

#include "openglapi.h"
#include "log.h"

std::string read_file(const std::string& filepath)
{
	std::ifstream file(filepath);
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	auto vertexCode = read_file(vertexShaderPath);
	auto fragmentCode = read_file(fragmentShaderPath);
	auto vertexSource = vertexCode.c_str();
	auto fragmentSource = fragmentCode.c_str();


	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vertexShader, 1, &vertexSource, nullptr));
	GLCall(glCompileShader(vertexShader));

	int success;
	char infoLog[512];
	GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog));
		LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog);
	}

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fragmentShader, 1, &fragmentSource, nullptr));
	GLCall(glCompileShader(fragmentShader));

	GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog));
		LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog);
	}

	m_rendererId = glCreateProgram();
	GLCall(glAttachShader(m_rendererId, vertexShader));
	GLCall(glAttachShader(m_rendererId, fragmentShader));
	GLCall(glLinkProgram(m_rendererId));

	GLCall(glGetProgramiv(m_rendererId, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLCall(glGetProgramInfoLog(m_rendererId, 512, nullptr, infoLog));
		LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog);
	}

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_rendererId));
}

void Shader::bind() const
{
	GLCall(glUseProgram(m_rendererId));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_rendererId, name.c_str()));
	if (location == -1)
		LOG_WARN("Warning: uniform '{}' doesn't exist!", name);

	m_uniformLocationCache[name] = location;
	return location;
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(getUniformLocation(name), value));
}