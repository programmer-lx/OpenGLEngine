#include "BufferObject.h"

#include <cassert>

BufferObject::BufferObject()
{
    glGenBuffers(1, &m_BufferID);
}

BufferObject::BufferObject(GLenum target)
    : BufferObject()
{
    m_Target = target;
}

BufferObject::BufferObject(GLenum target, std::int64_t bufferSize, const void* data, GLenum usage)
    : BufferObject()
{
    m_Target = target;

    bind();
    resetData(bufferSize, data, usage);
}

BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &m_BufferID);
}

void BufferObject::startAddSubData()
{
    m_Offset = 0;

    // bind buffer
    glBindBuffer(m_Target, m_BufferID);
}

void BufferObject::startAddSubData_BindBufferBase(std::uint32_t index)
{
    startAddSubData();
    glBindBufferBase(m_Target, index, m_BufferID);
}

void BufferObject::endAddSubData()
{
    // unbind buffer
    glBindBuffer(m_Target, 0);
}

void BufferObject::resetData(std::int64_t bufferSize, const void* data, GLenum usage)
{
    glBindBuffer(m_Target, m_BufferID);
    glBufferData(m_Target, bufferSize, data, usage);
}

void BufferObject::bind()
{
    glBindBuffer(m_Target, m_BufferID);
}

void BufferObject::unbind()
{
    glBindBuffer(m_Target, 0);
}

void BufferObject::addSubDataNative(const void* data, std::int64_t bufferSize)
{
    glBufferSubData(m_Target, m_Offset, bufferSize, data);
    m_Offset += bufferSize;
}

std::uint32_t BufferObject::bufferID() const
{
    return m_BufferID;
}
