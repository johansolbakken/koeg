#pragma once

#include <cstdint>

class VertexBuffer
{
public:
    VertexBuffer(const float *data, uint32_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

private:
    uint32_t m_rendererId;
};

class IndexBuffer
{
public:
    IndexBuffer(const uint32_t *data, uint32_t count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    uint32_t count() const;

private:
    uint32_t m_rendererId;
    uint32_t m_count;
};