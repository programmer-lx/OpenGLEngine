#pragma once

#include <cinttypes>

class BufferObject;

class VertexArrayObject final
{
private:
    std::uint32_t m_VAO_ID{};

public:
    VertexArrayObject();
    ~VertexArrayObject();

    std::uint32_t vaoID() const;

    void bind();
    void unbind();

    void setLayoutAttribute(std::int32_t layoutLocation, std::int32_t size, std::int32_t stride, std::uint64_t offset);
    void setDivisor(std::int32_t layoutLocation, std::int32_t divisor);

    void setupBatchDrawMatrixM(std::int32_t startLayoutLocation, BufferObject* instanceVBO, std::int32_t divisor = 1);
};
