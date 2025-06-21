#pragma once

#include <glad/glad.h>

#include <cinttypes>

class BufferObject final
{
private:
    std::uint32_t m_BufferID{};
    GLenum m_Target{};

    std::int64_t m_Offset{};

private:
    BufferObject();

public:
    explicit BufferObject(GLenum target);
    BufferObject(GLenum target, std::int64_t bufferSize, const void* data = nullptr, GLenum usage = GL_STATIC_DRAW);
    ~BufferObject();

    void startAddSubData();
    void startAddSubData_BindBufferBase(std::uint32_t index);
    void endAddSubData();

    void resetData(std::int64_t bufferSize, const void* data, GLenum usage = GL_STATIC_DRAW);
    void bind();
    void unbind();

    void addSubDataNative(const void* data, std::int64_t bufferSize);

    template <typename T>
    void addSubData(const void* data)
    {
        glBufferSubData(m_Target, m_Offset, sizeof(T), data);
        m_Offset += sizeof(T);
    }

    std::uint32_t bufferID() const;
};
