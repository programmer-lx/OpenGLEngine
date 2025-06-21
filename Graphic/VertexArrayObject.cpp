#include "VertexArrayObject.h"

#include "BufferObject.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

VertexArrayObject::VertexArrayObject()
{
    glGenVertexArrays(1, &m_VAO_ID);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &m_VAO_ID);
}

std::uint32_t VertexArrayObject::vaoID() const
{
    return m_VAO_ID;
}

void VertexArrayObject::bind()
{
    glBindVertexArray(m_VAO_ID);
}

void VertexArrayObject::unbind()
{
    glBindVertexArray(0);
}

void VertexArrayObject::setLayoutAttribute(std::int32_t layoutLocation, std::int32_t size, std::int32_t stride, std::uint64_t offset)
{
    glVertexAttribPointer(layoutLocation, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(layoutLocation);
}

void VertexArrayObject::setupBatchDrawMatrixM(std::int32_t startLayoutLocation, BufferObject* instanceVBO, std::int32_t divisor)
{
    instanceVBO->bind();

    setLayoutAttribute(startLayoutLocation, 4, sizeof(glm::mat4x4), 0);
    setLayoutAttribute(startLayoutLocation + 1, 4, sizeof(glm::mat4x4), sizeof(glm::vec4));
    setLayoutAttribute(startLayoutLocation + 2, 4, sizeof(glm::mat4x4), sizeof(glm::vec4) * 2);
    setLayoutAttribute(startLayoutLocation + 3, 4, sizeof(glm::mat4x4), sizeof(glm::vec4) * 3);

    setDivisor(startLayoutLocation, divisor);
    setDivisor(startLayoutLocation + 1, divisor);
    setDivisor(startLayoutLocation + 2, divisor);
    setDivisor(startLayoutLocation + 3, divisor);

    instanceVBO->unbind();
}

void VertexArrayObject::setDivisor(std::int32_t layoutLocation, std::int32_t divisor)
{
    glVertexAttribDivisor(layoutLocation, divisor);
}

