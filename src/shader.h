#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	~Shader();

	void bind() const;

	void unbind() const;

	void setUniform1i(const std::string& name, int value);

private:
	uint32_t m_rendererId;

	int getUniformLocation(const std::string& name);

	std::unordered_map<std::string, int> m_uniformLocationCache;
};