#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	~Shader();

	void bind() const;

	void unbind() const;

	void setUniform1i(const std::string& name, int value);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	uint32_t m_rendererId;

	int getUniformLocation(const std::string& name);

	std::unordered_map<std::string, int> m_uniformLocationCache;
};