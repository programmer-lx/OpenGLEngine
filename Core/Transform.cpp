#include "Transform.h"

#include "Misc/Math.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::vec3 Transform::s_WorldForward = glm::vec3(0, 0, -1);
glm::vec3 Transform::s_WorldRight = glm::vec3(1, 0, 0);
glm::vec3 Transform::s_WorldUp = glm::vec3(0, 1, 0);

void Transform::recomputeModelMatrix()
{
    m_ModelMatrix = Math::modelMatrix(m_Position, *this);
}

Transform::Transform()
{
    m_Position = glm::vec3(0, 0, 0);
    m_Rotation = glm::quat(1, 0, 0, 0);
    m_Scale = glm::vec3(1, 1, 1);
}

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
{
    m_Position = position;
    m_Rotation = rotation;
    m_Scale = scale;
}

Transform::Transform(const Transform& rhs)
{
    m_Position = rhs.m_Position;
    m_Rotation = rhs.m_Rotation;
    m_Scale = rhs.m_Scale;
}

Transform::Transform(Transform&& rhs) noexcept
{
    m_Position = rhs.m_Position;
    m_Rotation = rhs.m_Rotation;
    m_Scale = rhs.m_Scale;
}

bool Transform::operator==(const Transform& rhs) const
{
    return m_Position == rhs.m_Position
           && m_Rotation == rhs.m_Rotation
           && m_Scale == rhs.m_Scale;
}

bool Transform::operator!=(const Transform& rhs) const
{
    return !(*this == rhs);
}

Transform& Transform::operator=(const Transform& rhs)
{
    if (*this != rhs)
    {
        m_Position = rhs.m_Position;
        m_Rotation = rhs.m_Rotation;
        m_Scale = rhs.m_Scale;
    }
    return *this;
}

Transform& Transform::operator=(Transform&& rhs) noexcept
{
    if (*this != rhs)
    {
        m_Position = rhs.m_Position;
        m_Rotation = rhs.m_Rotation;
        m_Scale = rhs.m_Scale;
    }
    return *this;
}

void Transform::reset(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
{
    m_Position = position;
    m_Rotation = rotation;
    m_Scale = scale;
}

void Transform::setPosition(const glm::vec3& pos)
{
    m_Position = pos;
    m_Dirty = true;
}

void Transform::setRotation(const glm::quat& rotation)
{
    m_Rotation = rotation;
    m_Dirty = true;
}

void Transform::setScale(const glm::vec3& scale)
{
    m_Scale = scale;
    m_Dirty = true;
}

glm::vec3 Transform::forward() const
{
    return glm::normalize(m_Rotation * worldForward());
}

glm::vec3 Transform::right() const
{
    return glm::normalize(m_Rotation * worldRight());
}

glm::vec3 Transform::up() const
{
    return glm::normalize(m_Rotation * worldUp());
}

const glm::mat4x4& Transform::modelMatrix()
{
    if (m_Dirty)
    {
        recomputeModelMatrix();
        m_Dirty = false;
    }
    return m_ModelMatrix;
}

void Transform::localTranslate(const glm::vec3& offset)
{
    m_Position += m_Rotation * offset;
    m_Dirty = true;
}

void Transform::worldTranslate(const glm::vec3& offset)
{
    setPosition(m_Position + offset);
}

void Transform::localRotate(float angle, const glm::vec3& axis)
{
    glm::quat delta = glm::angleAxis(glm::radians(angle), axis);
    m_Rotation = glm::normalize(m_Rotation * delta);
    m_Dirty = true;
}

void Transform::worldRotate(float angle, const glm::vec3& axis)
{
    glm::quat delta = glm::angleAxis(glm::radians(angle), axis);
    m_Rotation = glm::normalize(delta * m_Rotation);
    m_Dirty = true;
}

void Transform::lookAt(const glm::vec3& target)
{
    lookAt(target, worldUp());
}

void Transform::lookAt(const glm::vec3& target, const glm::vec3& worldUp)
{
    m_Rotation = glm::quatLookAt(glm::normalize(target - m_Position), worldUp);
    m_Dirty = true;
}
