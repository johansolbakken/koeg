#pragma once

#include <string>
#include <cstdint>

class Shader
{
public:
    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    ~Shader();

    void bind() const;
    void unbind() const;

private:
    uint32_t m_rendererId;
};