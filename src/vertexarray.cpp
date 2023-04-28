#include "vertexarray.h"

#include "openglapi.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_rendererId));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_rendererId));
}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(m_rendererId));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
    bind();
    vertexBuffer->bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);
    unbind();
}

