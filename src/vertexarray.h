#pragma once

#include "buffer.h"

#include <memory>

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer);

private:
    uint32_t m_rendererId{};


};
